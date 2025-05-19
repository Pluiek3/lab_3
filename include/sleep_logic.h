#ifndef SLEEP_LOGIC_H
#define SLEEP_LOGIC_H

#include "time_utils.h"

typedef struct {
    Time bedtime;
    Time waketime;
} SleepData;

typedef struct {
    int hours;
    int minutes;
    const char *comment;
} SleepResult;

SleepResult calculate_sleep(SleepData data);

#endif
