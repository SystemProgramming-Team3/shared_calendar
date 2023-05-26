#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "calClient.h"

int flag = 0;
int year, month, day, startday;
int i = 0;
char* string;
char *ymd;
int *ret;
// int array[9] = {10, 11, 14, 0, 0, 0, 0, 0, 0};

int currentday = 1;

void displayTitle();
void displayCalendar();
void displayContentbox();
void displayContents();
void displayCommandline();
void displayUsagebox();
int *textbox();
char *getString(int *buf);
void getcurrent();
void getOther(int, int);
int day_of_week(int, int);
void printCal(WINDOW *win, void (*setOn)(WINDOW *));
void initwin();
void onCurrent(WINDOW *win);
void onOther(WINDOW *win);
void offCurrent(WINDOW *win);
void offOther(WINDOW *win);
void (*setOnFn)(WINDOW *) = &onCurrent;
void intArrayToCharArray(int *intArray, char *charArray, int length);
void setFunction(char *string);
void option_cc(char* origin,char* string);


int main()
{
    // 현재 시점 기준으로 string제작 => 서버와 통신하여 현재 년/월에서 데이터가 들어있는 날짜를 동기화해옴.
    getcurrent();
    char *current = malloc(sizeof(char) * 20);
    sprintf(current, "cc %04d.%02d ", year, month);
    ret = cc_client_func("127.0.0.1", "3000", current);

    ymd = malloc(sizeof(char)*20);
    sprintf(ymd,"%04d.%02d.%02d",year,month,day);
    
    char *stringcopy;
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    for (;;)
    {
        string = calloc(BUFSIZ, sizeof(char));
        i = 0;
        currentday = 1;
        initwin();// 기본틀만 나오도록
        string = getString(textbox()); // fullstring을 받아옴
        setFunction(string); // string처리와 분기문이 다 들어있다, 필요한 부분을 함수포인터 수정으로 동기화해줌
        
        endwin();
    }
    return 0;
}

void setFunction(char *string)
{ // => 문자열처리 따로 해주기!
    char* origin = malloc(sizeof(char)*20);
    strcpy(origin,string);
    char* option = malloc(sizeof(char)*10);
    option = strtok(string," "); //cc
    string = strtok(NULL," "); //2023.05
    //공통적으로는 여기까지 처리, 앞으로 플래그에 따라서 다르게 처리하면 될듯?

    if (!strcmp(option, "cc"))
        option_cc(origin,string);
    else if (!strcmp(option, "c"))
    {
        option_c(origin,string);
        //ymd = rest;
        //문자열 추가해주는 코드 작성
    }
    /*
    else if (!strcmp(option, "a"))
    {
    }
    else if (!strcmp(option, "mv"))
    {
    }
    else if (!strcmp(option, "rm")){}

    */
}

int day_of_week(int year, int month)
{ // 각 월의 1일이 언제인지 찾는 함수!
    int temp = 0;
    int i;
    for (i = 1; i < year; i++)
    {
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
            temp += 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            temp += 30;
            break;
        }
    }
    temp = temp + 1;
    return temp % 7;
}

void printCal(WINDOW *win, void (*setOn)(WINDOW *win))
{
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
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


void displayCalendar()
{
    WINDOW *win = newwin(12, 28, 1, 36);
    box(win, 0, 0);
    mvwprintw(win, 0, 10, "CALENDAR");
    mvwprintw(win, 2, 10, " %d/%d ", month, year);
    mvwprintw(win, 4, 3, " Su Mo Tu We Th Fr Sa ");
    printCal(win, setOnFn);
    wrefresh(win);
}

void displayContentbox()
{
    // CONTENTS
    WINDOW *win = newwin(26, 30, 1, 67);
    box(win, 0, 0);
    mvwprintw(win, 0, 11, "CONTENTS");
    wrefresh(win);
}

void displayContents() //c를 위해서 이걸 해결하고 c_client_func해석하면됨!
{
    WINDOW *win = newwin(24, 20, 2, 70);
    mvwprintw(win, 1, 7, ymd);
    int crow = 4;
    mvwprintw(win, crow, 0, "- birthday");
    mvwprintw(win, crow + 1, 0, "with my friend");
    crow += 3;
    mvwprintw(win, crow, 0, "- system programming");
    mvwprintw(win, crow + 1, 0, "quiz");

    wrefresh(win);
}

char *getString(int *buf)
{
    intArrayToCharArray(buf, string, BUFSIZ);
    return string;
}

// 수정X
void initwin()
{
    displayTitle();
    displayCalendar();
    displayContentbox();
    displayContents(); //처음에는 아무것도 없는 빈칸이다가. 계속해서 생기게 만들고 싶음
    displayCommandline();
    displayUsagebox();
}

int *textbox()
{
    int *buf = calloc(BUFSIZ, sizeof(int));
    WINDOW *text = newwin(1, 87, 29, 9);
    wrefresh(text);
    int ch;
    int pos = 0;
    int posch = pos;
    while (true)
    {
        ch = fgetc(stdin);
        if (ch != KEY_BACKSPACE && ch != 127)
        {
            mvwaddchstr(text, 0, pos, buf);
            if (ch != '\r' && ch != '\n')
                buf[i++] = ch;
            else
            {
                buf[i++] = 32;
                buf[i++] = '\0';
                delwin(text);
                return buf;
            }
            mvwaddch(text, 0, posch++, ch);
            wrefresh(text);
        }
        else if (i > 0)
        {
            i--;
            buf[i] = '\0';
            mvwdelch(text, 0, --posch);
            wrefresh(text);
        }
    }
}

void option_cc(char* origin,char* string){ //year,month,ret 은 전역변수 input "cc 2023.05 " "2023.05"
    char* fullstr = malloc(sizeof(20));
    strcpy(fullstr,string); //full str -> 2023.05, yearstr ->2023, monthstr ->05
    char* current = malloc(sizeof(20));
    char* yearstr = malloc(sizeof(10)); char* monthstr = malloc(sizeof(10));
    getcurrent();
    sprintf(current,"%04d.%02d",year,month);
    yearstr= strtok(string,".");
    monthstr = strtok(NULL,"");
    if (!strcmp(fullstr,current)) //받은 str와 현재 년/월이 같으면
    {
        ret = cc_client_func("127.0.0.1", "3000", origin);
        setOnFn = &onCurrent;
        sleep(5);
    }
    else
    {   
        ret = cc_client_func("127.0.0.1", "3000", origin);
        getOther(atoi(yearstr), atoi(monthstr));
        setOnFn = &onOther;
    }

}

void option_c(char* origin,char* string){
    //handling strings
    char* ccstring1 = malloc(sizeof(20));char* ccstring2 = malloc(sizeof(20));
    char* yearstr = malloc(sizeof(10)); char* monthstr = malloc(sizeof(10));char* daystr = malloc(sizeof(10));
    char* ccstring = malloc(sizeof(20));
    yearstr= strtok(string,".");
    monthstr = strtok(NULL,".");
    daystr = strtok(NULL,"");
    //strings for cc & cc command (달력화면 전환)
    sprintf(ccstring1,"cc %s.%s ",yearstr,monthstr);
    sprintf(ccstring2,"%s.%s",yearstr,monthstr);
    option_cc(ccstring1,ccstring2);
    //c command
}



//고정

void displayCommandline()
{
    // command
    WINDOW *box = newwin(3, 89, 28, 8);
    box(box, 0, 0);
    mvwprintw(box, 0, 3, "COMMAND");
    wrefresh(box);
}

void displayUsagebox()
{
    // USAGE
    WINDOW *win4 = newwin(13, 56, 14, 8);
    box(win4, 0, 0);
    mvwprintw(win4, 0, 4, "USAGE EXAMPLE");
    wattron(win4, A_REVERSE);
    mvwprintw(win4, 4, 7, "PRINT CONTENT");
    mvwprintw(win4, 2, 7, "PRINT CAL");
    mvwprintw(win4, 6, 7, "ADD CONTENT");
    mvwprintw(win4, 8, 7, "REMOVE CONTENT");
    mvwprintw(win4, 10, 7, "MOVE CONTENT");
    wattroff(win4, A_REVERSE);
    mvwprintw(win4, 4, 25, "c 2023.05.14");
    mvwprintw(win4, 2, 25, "cc 2023.05");
    mvwprintw(win4, 6, 25, "a 2023.05.14 key>content");
    mvwprintw(win4, 8, 25, "rm 2023.05.14 key");
    mvwprintw(win4, 10, 25, "mv 2023.05.14 2023.05.15 key");
    wrefresh(win4);
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
    int m = 0;
    while (ret[m] != 0)
    {
        if (currentday == ret[m++])
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
    int m = 0;
    while (ret[m] != 0)
    {
        if (currentday == ret[m++])
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

void displayTitle()
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

    wrefresh(title);
}

