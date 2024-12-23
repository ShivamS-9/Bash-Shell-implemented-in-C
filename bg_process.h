#ifndef __BG_PROCESS_H
#define __BG_PROCESS_H
// System Libraries
// User Defined Libraries
#include "headers.h"

void store_process (int pid, char* name);
typedef struct bg_process
{
    int pid_proc;
    char* name_proc;
    struct bg_process* next;
    struct bg_process* prev;
}proc;
void check_child_status();
void rem_bg_to_fg(int pid);
char* get_proc_name(int pid);
void store_fg(char* name, int pid);
void free_fg();
extern proc* fg_proc;

#endif