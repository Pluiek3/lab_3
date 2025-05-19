#include "../include/time_utils.h"
#include <stdio.h>
#include <stdlib.h>

Time parse_time(const char *time_str) {
    Time t = {0};
    sscanf(time_str, "%d:%d", &t.hours, &t.minutes);
    return t;
}

int time_to_minutes(Time t) {
    return t.hours * 60 + t.minutes;
}

Time minutes_to_time(int total_minutes) {
    Time t;
    t.hours = (total_minutes / 60) % 24;
    t.minutes = total_minutes % 60;
    return t;
}
