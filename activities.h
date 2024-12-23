#ifndef __ACTIVITIES_H
#define __ACTIVITIES_H
// System Libraries
// User Defined Libraries
#include "headers.h"
typedef struct activities_bg
{
    int pid_proc;
    char* name_proc;
}activity;
int comparator_1(const void *a, const void *b);
void store_activities(int pid, char* name);
int activities_function();
void kill_all();

#endif