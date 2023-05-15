#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

int year, month, day, startday;

void getcurrent();
int day_of_week(int, int);
void printCal(WINDOW *win);
void initwin();

int main()
{
    // 밑의 일련의 과정 전부 함수화하고,
    // 서버로 2023.09 (string)형태의 인자 넘겨주면
    // 받아온 응답에 해당하는 날짜만 마킹
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    for (;;)
        initwin();

    endwin();

    char string[30];

    return 0;
}

void initwin()
{

    WINDOW *win = newwin(12, 28, 1, 30);
    box(win, 0, 0);
    mvwprintw(win, 0, 10, "CALENDAR");
    getcurrent();
    mvwprintw(win, 2, 10, " %d/%d ", month, year);
    mvwprintw(win, 4, 3, " Su Mo Tu We Th Fr Sa ");
    printCal(win);

    WINDOW *win2 = newwin(25, 30, 1, 60);
    box(win2, 0, 0);
    mvwprintw(win2, 0, 12, "CONTENTS");

    WINDOW *win3 = newwin(3, 88, 26, 2);
    box(win3, 0, 0);
    mvwprintw(win3, 0, 3, "COMMAND");

    WINDOW *win4 = newwin(13, 56, 13, 2);
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
    mvwprintw(win4, 10, 25, "mv 2023.05.14 2023.05.15");

    refresh();
    wrefresh(win);
    wrefresh(win2);
    wrefresh(win3);
    wrefresh(win4);
    getch();
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