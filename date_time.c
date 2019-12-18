#include <time.h>
#include "date_time.h"
#include <unistd.h>
#include "command_window.h"

void update_date_time() {
    current_time = time(NULL);
    struct tm c_tm = *localtime(&current_time);
    year = c_tm.tm_year + 1900;
    printyear = year%100;
    month = c_tm.tm_mon+1;
    day = c_tm.tm_mday;
    hour = c_tm.tm_hour;
    minute = c_tm.tm_min;
}

int isleap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int mthdays(int month, int year)
{
    static const int days[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int leap = isleap(year);

    return days[leap][month];
}

int dayofweek(int d, int m, int y) 
{ 
    int weekday  = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
    return weekday;
} 

void delay(float seconds) {
    int time = (int) (CLOCKS_PER_SEC * seconds);

    clock_t start_time = clock();
    while (clock() < start_time + time);
}
