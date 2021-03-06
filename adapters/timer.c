// Copyright (C) Microsoft Corporation. All rights reserved.

#include "windows.h"

#include "azure_c_util/gballoc.h"
#include "azure_c_util/xlogging.h"

#include "azure_c_util/timer.h"

typedef struct TIMER_HANDLE_DATA_TAG
{
    LARGE_INTEGER freq;
    LARGE_INTEGER startTime;
}TIMER_HANDLE_DATA;

TIMER_HANDLE timer_create(void)
{
    TIMER_HANDLE result;
    result = malloc(sizeof(TIMER_HANDLE_DATA));
    if (result == NULL)
    {
        LogError("failure in malloc");
        /*return as is*/
    }
    else
    {
        (void)QueryPerformanceFrequency(&(result->freq)); /* from MSDN:  On systems that run Windows XP or later, the function will always succeed and will thus never return zero.*/
                                                          /*return as is*/
        (void)QueryPerformanceCounter(&(result->startTime)); /*from MSDN:  On systems that run Windows XP or later, the function will always succeed and will thus never return zero.*/
    }
    return result;
}

void timer_start(TIMER_HANDLE timer)
{
    if (timer == NULL)
    {
        LogError("invalid arg TIMER_HANDLE timer=%p", timer);
    }
    else
    {
        (void)QueryPerformanceCounter(&(timer->startTime)); /*from MSDN:  On systems that run Windows XP or later, the function will always succeed and will thus never return zero.*/
    }
}

double timer_get_elapsed(TIMER_HANDLE timer)
{
    double result;
    if (timer == NULL)
    {
        LogError("invalid arg TIMER_HANDLE timer=%p", timer);
        result = -1.0;
    }
    else
    {
        LARGE_INTEGER stopTime;
        (void)QueryPerformanceCounter(&stopTime);
        result = ((double)(stopTime.QuadPart - timer->startTime.QuadPart) / (double)timer->freq.QuadPart);
    }
    return result;
}

double timer_get_elapsed_ms(TIMER_HANDLE timer)
{
    double result = timer_get_elapsed(timer);
    return result < 0 ? result : result * 1000;
}

void timer_destroy(TIMER_HANDLE timer)
{
    if (timer == NULL)
    {
        LogError("invalid arg TIMER_HANDLE timer=%p", timer);
    }
    else
    {
        free(timer);
    }
}

static LARGE_INTEGER g_freq;
static volatile LONG g_timer_state = 0; /*0 - not "created", 1 - "created", "2" - creating*/

/*returns a time in ms since "some" start.*/
double timer_global_get_elapsed_ms(void)
{
    while (InterlockedCompareExchange(&g_timer_state, 2, 0) != 1)
    {
        (void)QueryPerformanceFrequency(&g_freq); /*from MSDN:  On systems that run Windows XP or later, the function will always succeed and will thus never return zero.*/
        (void)InterlockedExchange(&g_timer_state, 1);
    }

    LARGE_INTEGER now;
    (void)QueryPerformanceCounter(&now);
    return (double)now.QuadPart * 1000.0 / (double)g_freq.QuadPart;
}

