#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <calClient.h>

// ip,port,커맨드 들어있는 int string

int flag = 0;
int year, month, day, startday;
int i = 0;
int *buf;
char string[1024];
int array[3] = {10, 11, 14};
int currentday = 1;

void getcurrent();
void getOther(int, int);
int day_of_week(int, int);
void printCal(WINDOW *win, void (*setOn)(WINDOW *));
void initwin(); // ccwin,cwin 등 필요한 함수 함수포인트
void onCurrent(WINDOW *win);
void onOther(WINDOW *win);
void offCurrent(WINDOW *win);
void offOther(WINDOW *win);
void (*setOnFn)(WINDOW *) = &onCurrent;
void intArrayToCharArray(int *intArray, char *charArray, int length);
int main()
{

    buf = malloc(sizeof(int) * 1024);
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    // int filedes2 = creat("./creat.txt", 0644);

    for (;;)
    {
        i = 0;
        currentday = 1;
        initwin(); // 이걸 함수포인터로 만들고 싶음. win.h라는 헤더파일 선언해서 함수포인트의 인자가되는 함수들 정의해두면 좋을거같음.
        /*
        c 명령어는 함수의 content 표현하는 부분 함수화
        cc 명령어는 캘린더 생성하는 부분 함수화
        add content,remove,move content는 캘린더 생성 + content 함수화!

        서버에게서 데이터 받아오기
        */
        // write(filedes2, buf, 30);

        intArrayToCharArray(buf, string, 1024);
        printf("[%s]", string);
        char *str = strtok(string, " ");
        printf("%s\n\n", str);

        int *ret = malloc(sizeof(int) * 31);
        if (!strcmp(str, "cc"))
        {
            ret = cc_client_func("127.0.0.1", "3000", string + 3);
            for (int k = 0; k < 10; k++)
                printf("[%d]", ret[i]);
        }
        buf = calloc(1024, sizeof(int));
        string[0] = '\0';

        sleep(5);
        endwin();

        // 버퍼 내용을 서버에 보내고 통신
        // calserver실행 후, ip 127.0.0.1, port 3000(calserver실행 시 지정된 포트번호 넣어주기)
        //  ret = cc_client_func("127.0.0.1", "3000", buf);

        // strtok후, 명령어 분류 -> 맞는 함수 쓰고
        // win 정보를 갱신하는 코드를 넣어주기
    }

    return 0;
}

void initwin()
{
    WINDOW *title = newwin(20, 30, 0, 12);

    mvwprintw(title, 1, 0, " _____   ___   _  ");
    mvwprintw(title, 2, 0, "/  __ \\ / _ \\ | |");
    mvwprintw(title, 3, 0, "| /  \\// /_\\ \\| | ");
    mvwprintw(title, 4, 0, "| |    |  _  || |");
    mvwprintw(title, 5, 0, "| \\__/\\| | | || |____");
    mvwprintw(title, 6, 0, " \\____/\\_| |_/\\_____/");
    mvwprintw(title, 7, 4, " _   _  _____ ");
    mvwprintw(title, 8, 4, "| | | |/  ___|");
    mvwprintw(title, 9, 4, "| | | |\\ `--. ");
    mvwprintw(title, 10, 4, "| | | | `--. \\");
    mvwprintw(title, 11, 4, "| |_| |/\\__/ /");
    mvwprintw(title, 12, 4, " \\___/ \\____/ ");

    WINDOW *win = newwin(12, 28, 1, 36);
    box(win, 0, 0);
    mvwprintw(win, 0, 10, "CALENDAR");

    getcurrent(); // cc 2023.07->getotherCal();
    // getOther(2023, 10);
    mvwprintw(win, 2, 10, " %d/%d ", month, year);
    mvwprintw(win, 4, 3, " Su Mo Tu We Th Fr Sa ");
    printCal(win, setOnFn);

    // CONTENTS
    WINDOW *win2 = newwin(26, 30, 1, 67);
    box(win2, 0, 0);
    mvwprintw(win2, 0, 12, "CONTENTS");

    // 내용출력코드
    mvwprintw(win2, 2, 10, "2023.05.23");

    int crow = 4;
    mvwprintw(win2, crow, 4, "- birthday");
    mvwprintw(win2, crow + 1, 4, "with my friend");
    crow += 3;

    mvwprintw(win2, crow, 4, "- system programming");
    mvwprintw(win2, crow + 1, 4, "quiz");

    // command
    WINDOW *win3box = newwin(3, 89, 28, 8);
    box(win3box, 0, 0);
    mvwprintw(win3box, 0, 3, "COMMAND");

    WINDOW *win3 = newwin(1, 87, 29, 9);

    // USAGE
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
    int pos = 0;
    int posch = pos;
    while (true)
    {
        ch = fgetc(stdin);
        if (ch != KEY_BACKSPACE && ch != 127)
        {
            mvwaddchstr(win3, 0, pos, buf);
            if (ch != '\r')
                buf[i++] = ch;
            else
                buf[i++] = '\0';
            mvwaddch(win3, 0, posch++, ch);
            // wmove(win3, 0, posch);
            wrefresh(win3);
        }
        else if (i > 0)
        {
            i--;
            buf[i] = '\0';
            mvwdelch(win3, 0, --posch);
            // wmove(win3, 0, posch);
            wrefresh(win3);
        }

        if ((ch) == '\r')
            break;
    }
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

void printCal(WINDOW *win, void (*setOn)(WINDOW *win))
{
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    // wattron(win, COLOR_PAIR(1));
    int row = 5;
    int col = 1;
    int startday = day_of_week(year, month);
    col += startday * 3;

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

    while (currentday <= days_in_month)
    {
        (*setOn)(win);
        mvwprintw(win, row, col + 3, "%2d", currentday);
        if (flag)
        {
            offCurrent(win);
        }
        else
        {
            offCurrent(win);
        }

        if (currentday == day && currentday < 9)
            col += 4;
        else
            col += 3;

        if (col >= 21)
        {
            row++;
            col = 1;
        }
        currentday++;
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

void getOther(int YEAR, int MONTH)
{
    year = YEAR;
    month = MONTH;
}

void onCurrent(WINDOW *win)
{
    int isHighlighted = 0;
    // while(array[i]!=0)
    for (int i = 0; i < 3; i++)
    {
        if (currentday == array[i])
        {
            isHighlighted = 1;
            break;
        }
    }

    if (currentday == day)
    {
        if (isHighlighted)
            wattron(win, COLOR_PAIR(1) | A_REVERSE);
        else
            wattron(win, A_REVERSE);
    }
    else if (isHighlighted)
    {
        wattron(win, COLOR_PAIR(1));
    }
}

void onOther(WINDOW *win)
{
    int isHighlighted = 0;
    for (int i = 0; i < 3; i++)
    {
        if (currentday == array[i])
        {
            isHighlighted = 1;
            break;
        }
    }

    if (isHighlighted)
    {
        wattron(win, COLOR_PAIR(1));
    }
}

void offCurrent(WINDOW *win)
{
    wattroff(win, COLOR_PAIR(1));
    wattroff(win, A_REVERSE);

    /*    if (current_day == day && isHighlighted)
            wattroff(win, COLOR_PAIR(1) && A_REVERSE);
        else if (current_day == day)
            wattroff(win, A_REVERSE);
        else if (isHighlighted)
            wattroff(win, COLOR_PAIR(1));
            */
}

void offOther(WINDOW *win)
{
    wattroff(win, COLOR_PAIR(1));
}

void intArrayToCharArray(int *intArray, char *charArray, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        sprintf(charArray + (i * sizeof(char)), "%c", intArray[i]);
    }
}