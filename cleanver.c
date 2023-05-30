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
char *string;
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
int findFirstday(int, int);
void printCal(WINDOW *win, void (*setOn)(WINDOW *));
void initwin();
void onCurrent(WINDOW *win);
void onOther(WINDOW *win);
void offCurrent(WINDOW *win);
void offOther(WINDOW *win);
void (*setOnFn)(WINDOW *) = &onCurrent;
void intArrayToCharArray(int *intArray, char *charArray, int length);
void setFunction(char *string);
int *(*clientfunc)(char *, char *, char *) = &cc_client_func;

typedef struct
{
    char **key;
    char **content;
    int num;
} KeyContent;
KeyContent contentData;

typedef struct
{
    char *yearmonth;
    char *yearmonthday;
    char *yearmonthday2;
    char *key;
    char *content;
    char *yearstr;
    char *monthstr;
    char *daystr;
} Strings; // 각각 분기에서 handling하는 함수 만들어서 필요할 때 사용

KeyContent getKeyContent(const char *cstring) // cstring을 받아서 c명령어를 수행하고 key, content배열내에 content를 담아두는 함수
{
    KeyContent result;

    result.key = malloc(sizeof(char *) * 20);
    result.content = malloc(sizeof(char *) * 20);

    for (int i = 0; i < 20; i++)
    {
        result.key[i] = malloc(sizeof(char) * 100);
        result.content[i] = malloc(sizeof(char) * 100);
    }

    char *cdret = c_client_func("127.0.0.1", "3000", cstring);
    char *ptr = malloc(sizeof(char) * 100);
    result.num = 0;

    ptr = strtok(cdret, "\n");

    while (ptr != NULL)
    {
        if (result.num % 2 == 0)
        {
            strcpy(result.key[result.num / 2], ptr);
        }
        else
        {
            strcpy(result.content[result.num / 2], ptr);
        }
        result.num++;
        ptr = strtok(NULL, "\n");
    }

    free(ptr);
    return result;
}

void option_cc(char *string)
{
    char *string2 = malloc(20);
    char *origin = malloc(30);
    strcpy(string2, string);
    // 비교할 currentstring
    char *current = malloc(20);
    getcurrent();
    sprintf(current, "%04d.%02d", year, month);

    char *yearstr = strtok(string, ".");
    char *monthstr = strtok(NULL, "");
    sprintf(origin, "cc %s ", string2);

    if (!strcmp(string2, current)) // 받은 str와 현재 년/월이 같으면
    {
        ret = (*clientfunc)("127.0.0.1", "3000", origin);
        setOnFn = &onCurrent;
    }
    else
    {
        ret = (*clientfunc)("127.0.0.1", "3000", origin);
        getOther(atoi(yearstr), atoi(monthstr));
        setOnFn = &onOther;
    }
}

void option_c(char *string)
{ // cc와 c를 동시에
    Strings data;
    data.yearmonthday = malloc(30);
    data.yearstr = malloc(10);
    data.monthstr = malloc(10);
    data.daystr = malloc(10);
    data.yearmonth = malloc(20);
    char *cstring = malloc(30);
    strcpy(data.yearmonthday, string);
    data.yearstr = strtok(string, ".");
    data.monthstr = strtok(NULL, ".");
    data.daystr = strtok(NULL, "");
    sprintf(data.yearmonth, "%s.%s", data.yearstr, data.monthstr);
    sprintf(cstring, "c %s ", data.yearmonthday);
    strcpy(ymd, data.yearmonthday);
    option_cc(data.yearmonth);
    contentData = getKeyContent(cstring);
}

void option_a(char *string)
{
    Strings data;
    char *origin = malloc(100);
    data.yearmonthday = strtok(string, " ");
    char *string2 = malloc(40);
    strcpy(string2, data.yearmonthday);
    data.key = malloc(20);
    data.content = malloc(40);
    data.key = strtok(NULL, ">");
    data.content = strtok(NULL, "");
    data.yearstr = malloc(10);
    data.monthstr = malloc(10);
    data.yearstr = strtok(string2, ".");
    data.monthstr = strtok(NULL, ".");
    //
    char *current = malloc(20);
    char *input = malloc(20);
    getcurrent();
    sprintf(current, "%04d.%02d", year, month);
    sprintf(input, "%s.%s", data.yearstr, data.monthstr);
    //
    sprintf(origin, "a %s %s>%s ", data.yearmonthday, data.key, data.content);
    char *cstring = malloc(30);

    if (!strcmp(input, current)) // 받은 str와 현재 년/월이 같으면
    {
        ret = a_client_func("127.0.0.1", "3000", origin);
        setOnFn = &onCurrent;
        option_c(data.yearmonthday);
    }
    else
    {
        ret = a_client_func("127.0.0.1", "3000", origin);
        getOther(atoi(data.yearstr), atoi(data.monthstr));
        setOnFn = &onOther;
        option_c(data.yearmonthday);
    }
    free(origin);
    free(input);
    free(current);
}

void option_mv(char *string)
{ // yearmonthday yearmonthday2 key
    Strings data;
    data.yearmonthday = malloc(20);
    data.yearmonthday2 = malloc(20);
    data.key = malloc(40);
    data.yearstr = malloc(5);
    data.monthstr = malloc(5);
    data.daystr = malloc(5);
    data.yearmonthday = strtok(string, " ");
    data.yearmonthday2 = strtok(NULL, " ");
    data.key = strtok(NULL, "");
    char *string2 = malloc(20);
    strcpy(string2, data.yearmonthday2);
    data.yearstr = strtok(string2, ".");
    data.monthstr = strtok(NULL, ".");
    data.daystr = strtok(NULL, "");
    char *input = malloc(30);
    getcurrent();
    char *current = malloc(30);
    char *origin = malloc(150);
    sprintf(current, "%04d.%02d", year, month);
    sprintf(input, "%s.%s", data.yearstr, data.monthstr);
    sprintf(origin, "mv %s %s %s ", data.yearmonthday, data.yearmonthday2, data.key);

    printf("%s %s %s ", current, input, origin);
    if (!strcmp(input, current)) // 옮기고자 하는 날
    {
        ret = mv_client_func("127.0.0.1", "3000", origin);
        setOnFn = &onCurrent;
        // option_c(data.yearmonthday2);
    }
    else
    {
        ret = mv_client_func("127.0.0.1", "3000", origin);
        getOther(atoi(data.yearstr), atoi(data.monthstr));
        setOnFn = &onOther;
        // option_c(data.yearmonthday2);
    }
}

void option_rm(char *string)
{
    Strings data;
    data.yearmonthday = strtok(string, " ");
    data.key = strtok(NULL, "");
    printf("%s %s", data.yearmonthday, data.key);
    char *string2 = malloc(10);
    strcpy(string2, data.yearmonthday);
    data.yearstr = strtok(string2, ".");
    data.monthstr = strtok(NULL, ".");
    char *current = malloc(10);
    sprintf(current, "%04d.%02d", year, month);
    char *input = malloc(10);
    sprintf(input, "%s.%s", data.yearstr, data.monthstr);
    char *origin = malloc(40);
    sprintf(origin, "rm %s %s ", data.yearmonthday, data.key);
    if (!strcmp(input, current))
    {
        ret = rm_client_func("127.0.0.1", "3000", origin);
        setOnFn = &onCurrent;
        option_c(data.yearmonthday);
    }
    else
    {
        ret = rm_client_func("127.0.0.1", "3000", origin);
        getOther(atoi(data.yearstr), atoi(data.monthstr));
        setOnFn = &onOther;
        option_c(data.yearmonthday);
    }
}

int main()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGKILL, SIG_IGN);
    getcurrent();
    // default
    clientfunc = &cc_client_func;
    char *current = malloc(sizeof(30));
    ymd = malloc(sizeof(20));
    sprintf(current, "%04d.%02d.%02d", year, month, day);
    strcpy(ymd, current);
    option_c(current);

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    for (;;)
    {
        string = calloc(BUFSIZ, sizeof(char));
        currentday = 1;
        initwin();                     // 기본틀만 나오도록
        string = getString(textbox()); // fullstring을 받아옴
        setFunction(string);           // string처리와 분기문이 다 들어있다, 필요한 부분을 함수포인터 수정으로 동기화해줌

        endwin();
    }
    return 0;
}

void setFunction(char *string)
{
    char *option = malloc(5);
    option = strtok(string, " ");
    string = strtok(NULL, "");
    char *cstring = malloc(30);
    if(!strcmp(option, "q"))
    {
        endwin();
        exit(0);
    }
    else if (!strcmp(option, "cc"))
    {
        clientfunc = &cc_client_func;
        option_cc(string);
    }
    else if (!strcmp(option, "c"))
    {
        option_c(string);
    }
    else if (!strcmp(option, "a"))
    {
        option_a(string); // yearmonthday key>content
    }
    else if (!strcmp(option, "mv"))
    {
        option_mv(string); // yearmonthday yearmonthday2 key
    }
    else if (!strcmp(option, "rm"))
    {
        option_rm(string); // yearmonthday key
    }
}

void displayContents()
{
    WINDOW *win = newwin(24, 20, 2, 70);
    mvwprintw(win, 1, 7, ymd);
    char *cstring = malloc(30);
    sprintf(cstring, "c %s ", ymd);

    int crow = 4;

    for (int i = 0; i < contentData.num / 2; i++)
    {
        mvwprintw(win, crow, 0, contentData.key[i]);
        mvwprintw(win, crow + 1, 0, contentData.content[i]);
        crow += 3;
    }

    /*
        mvwprintw(win, crow, 0, "- birthday");
        mvwprintw(win, crow + 1, 0, "with my friend");
        crow += 3;
        mvwprintw(win, crow, 0, "- system programming");
        mvwprintw(win, crow + 1, 0, "quiz");
    */
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
    displayContents(); // 처음에는 아무것도 없는 빈칸이다가. 계속해서 생기게 만들고 싶음
    displayCommandline();
    displayUsagebox();
}

// 고정

int *textbox()
{
    int i = 0;
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
                // buf[i++] = 32;
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

int findFirstday(int year, int month)
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
        {
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
    int startday = findFirstday(year, month);
    col += startday * 3;
    int days = 31;
    switch (month)
    {
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            days = 29;
        else
            days = 28;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        days = 30;
        break;
    }

    while (currentday <= days)
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

void displayCalendar() // 전역변수인, year,month 사용 printCal
{
    WINDOW *win = newwin(12, 28, 1, 36);
    box(win, 0, 0);
    mvwprintw(win, 0, 10, "CALENDAR");
    mvwprintw(win, 2, 10, " %d/%d ", month, year);
    mvwprintw(win, 4, 3, " Su Mo Tu We Th Fr Sa ");
    printCal(win, setOnFn);
    wrefresh(win);
}

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
