#include <stdio.h>
#include <time.h>

int day_of_week(int, int);
void print_calendar(int, int, int);
void print_head(int, int);

int main(void)
{
        int year, month, day; // day는 해당월 1월의 요일 (월1, 화2, 수3 , ...)
        struct tm *tmp;

        // 현재시각 기준으로 달력 display 1회
        time_t curtime = time(NULL);
        tmp = localtime(&curtime);
        year = tmp->tm_year + 1900;
        month = tmp->tm_mon + 1;

        print_head(year, month);
        print_calendar(day_of_week(year, month), year, month);

        // 기입년월 기준으로 달력 display(반복문은 확인 위해 삽입)
        do
        {
                printf("캘린더 이동 (년, 월 기입)  ");
                scanf("%d%d", &year, &month); // 년월 입력
                if (month >= 1 && month <= 12)
                {

                        print_head(year, month);                               // 달력 상단부 출력
                        print_calendar(day_of_week(year, month), year, month); // 달력 날짜 출력
                }
                else
                        printf("잘못된 입력입니다\n>> ");

        } while (1);

        return 0;
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

/****************************/

void print_calendar(int sd, int year, int month)
{
        int i, j; // for문
        int temp; // 해달 월의 총 일수

        switch (month)
        { // 한달의 일수를 구한다

        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
                temp = 31; // 한달이 31일인 경우
                break;

        case 4:
        case 6:
        case 9:
        case 11:

                temp = 30; // 한달이 30일인 경우
                break;
        case 2: // 2월의 경우
                if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
                        temp = 29;
                else
                        temp = 28;
        }
        printf("\t      ");
        for (i = 1; i <= sd; i++)
                printf("         ");
        j = sd;
        for (i = 1; i <= temp; i++)
        {
                if (j == 7)
                { // 요일별로 출력되도록 줄바꿈
                        printf("\n\n\n\n\n\t      ");
                        j = 0;
                }
                printf("%2d       ", i);
                j++;
        }
        printf("\n\n");
}

void print_head(int year, int month)
{
        printf("\n\t\t\t\t[%dY %02dM]\n\n", year, month);
        printf("\t      SUN      MON      TUE      WED      THU       FRI      SAT\n");
}
