#ifndef DATE_TIME_H
#define DATE_TIME_H
int printyear, year, month, day, hour, minute;
time_t current_time;

int isleap(int year);
int mthdays(int month, int year);
void update_date_time();
void delay(float seconds);
int dayofweek(int d, int m, int y);

#endif
