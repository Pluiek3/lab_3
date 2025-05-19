#ifndef TIME_UTILS_H
#define TIME_UTILS_H

typedef struct {
    int hours;
    int minutes;
} Time;

Time parse_time(const char *time_str);
int time_to_minutes(Time t);
Time minutes_to_time(int total_minutes);

#endif
