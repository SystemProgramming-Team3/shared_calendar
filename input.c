#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

//ip,port,커맨드 들어있는 int string

int year, month, day, startday;
 int i = 0;int buf[1024];int string[1024];

void getcurrent();
int day_of_week(int, int);
void printCal(WINDOW *win);
void initwin();

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int filedes2 = creat("./creat.txt",0644);


    for(;;){    
        initwin(); //이걸 함수포인터로 만들고 싶음. win.h라는 헤더파일 선언해서 함수포인트의 인자가되는 함수들 정의해두면 좋을거같음.
        /*
        c 명령어는 함수의 content 표현하는 부분 함수화
        cc 명령어는 캘린더 생성하는 부분 함수화
        add content,remove,move content는 캘린더 생성 + content 함수화!
        */
        //printf("[%ls]",buf); //이걸 서버와 통신해서 서버의 리턴값을 받아오는 함수로 만들고 싶다. 서버에 buf로 받아온 스트링을 주기, 필요하면 데이터 가공까지
        /*
        서버에게서 데이터 받아오기
        */
        write(filedes2, buf, 30);
        buf[0] = '\0';
        endwin();
    }

    return 0;
}

void initwin()
{
    WINDOW *title = newwin(20,30,0,12);
    mvwprintw(title, 1,0, " _____   ___   _  ");
    mvwprintw(title, 2,0, "/  __ \\ / _ \\ | |");
    mvwprintw(title, 3,0, "| /  \\// /_\\ \\| | ");
    mvwprintw(title, 4,0, "| |    |  _  || |");
    mvwprintw(title, 5,0, "| \\__/\\| | | || |____");
    mvwprintw(title, 6,0, " \\____/\\_| |_/\\_____/");
    mvwprintw(title, 7,4, " _   _  _____ ");
    mvwprintw(title, 8,4, "| | | |/  ___|");
    mvwprintw(title, 9,4, "| | | |\\ `--. ");
    mvwprintw(title, 10,4, "| | | | `--. \\");
    mvwprintw(title, 11,4, "| |_| |/\\__/ /");
    mvwprintw(title, 12,4, " \\___/ \\____/ ");
    

    WINDOW *win = newwin(12, 28, 1, 36);
    box(win, 0, 0);
    mvwprintw(win, 0, 10, "CALENDAR");
    getcurrent(); // cc 2023.07 -> getotherCal();
    mvwprintw(win, 2, 10, " %d/%d ", month, year);
    mvwprintw(win, 4, 3, " Su Mo Tu We Th Fr Sa ");
    printCal(win);

    WINDOW *win2 = newwin(26, 30, 1, 67);
    box(win2, 0, 0);
    mvwprintw(win2, 0, 12, "CONTENTS");

    WINDOW *win3box = newwin(3, 89, 28, 8);
    box(win3box,0, 0);
    mvwprintw(win3box, 0, 3, "COMMAND");

    WINDOW *win3 = newwin(1, 87, 29, 9);
  

    WINDOW *win4 = newwin(13, 56, 14, 8);
    box(win4, 0, 0);
    mvwprintw(win4, 0, 4, "USAGE EXAMPLE");
    wattron(win4, A_REVERSE);
    mvwprintw(win4, 2, 7, "PRINT CONTENT");
    mvwprintw(win4, 4, 7, "PRINT CAL");
    mvwprintw(win4, 6, 7, "ADD CONTENT");
    mvwprintw(win4, 8, 7, "REMOVE CONTENT");
    mvwprintw(win4, 10, 7, "MOVE CONTENT");
    wattroff(win4, A_REVERSE);
    mvwprintw(win4, 2, 25, "c 2023.05.14");
    mvwprintw(win4, 4, 25, "cc 2023.05");
    mvwprintw(win4, 6, 25, "a 2023.05.14 key>content");
    mvwprintw(win4, 8, 25, "rm 2023.05.14 key");
    mvwprintw(win4, 10, 25, "mv 2023.05.14 2023.05.15 key");
    
    refresh();
    wrefresh(title);
    wrefresh(win);
    wrefresh(win2);
    wrefresh(win3box);
    wrefresh(win4);
    wrefresh(win3);
    
    int ch;
    int pos = 0; int posch = pos;
    while((ch=fgetc(stdin))!='\r'){
        if (ch != KEY_BACKSPACE && ch != 127) {
            mvwaddchstr(win3,0,pos,buf);
            buf[i++] = ch;
            mvwaddch(win3, 0, posch++, ch);
            wmove(win3,1,posch);
            wrefresh(win3);
            
        }
        else if (i > 0) { 
            i--;
            buf[i] = '\0';  // remove the last character from the string
            mvwdelch(win3,0,--posch);
            wmove(win3,0,posch);
            wrefresh(win3);
        }
        
    }

    //구현해야할것 -> 캐리지리턴들어갔을때 scr끼워맞추는거,  
    //개행문자 입력받으면 서버에 buf데이터 가공후 넘겨주고, 서버의 응답을 받아 gui 재가공
}

int day_of_week(int year, int month)
{
    int temp = 0;
    int i;

    for (i = 1; i < year; i++)
    { // 년도별 일수
        if ((i % 4 == 0) && (i % 100 != 0) || (i % 400 == 0))
            temp += 366;
        else
            temp += 365;
    }

    for (i = 1; i < month; i++)
    {
        if (i == 2)
        { // 2월일경우 윤년 검사
            if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
                temp += 29;
            else
                temp += 28;
        }
        switch (i)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            temp += 31; // 한달이 31일인 경우
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            temp += 30; // 한달이 30일인 경우
            break;
        }
    }
    temp = temp + 1; // 마지막으로 일수를 더해 총 일 수를 구한다
    return temp % 7; // 1=월,2=화...6=토,0=일
}

void printCal(WINDOW *win)
{
    int row = 5;
    int col = 2;
    int days_in_month = 31;

    switch (month)
    {
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            days_in_month = 29;
        else
            days_in_month = 28;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        days_in_month = 30;
        break;
    }
    int startday = day_of_week(year, month);
    int current_day = 1;
    col += startday * 2;
    while (current_day <= days_in_month)
    {
        // 받은 특정 날짜들 파싱해온 상태에서
        // if current_day == theday -> 색깔넣는 등 강조
        if (current_day == day)
        {
            wattron(win, A_REVERSE);
        }
        mvwprintw(win, row, col + 3, "%2d", current_day);
        wattroff(win, A_REVERSE);
        if (current_day == day && current_day < 9)
        {
            col += 4;
        }
        else
        {
            col += 3;
        }
        if (col >= 21)
        {
            row++;
            col = 1;
        }
        current_day++;
    }
}

void getcurrent()
{
    struct tm *time_info;
    time_t current_time = time(NULL);
    time_info = localtime(&current_time);
    year = time_info->tm_year + 1900;
    month = time_info->tm_mon + 1;
    day = time_info->tm_mday;
}
