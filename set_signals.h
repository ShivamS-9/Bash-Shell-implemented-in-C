#ifndef __SIGNALS_H
#define __SIGNALS_H
// System Libraries
// User Defined Libraries
#include "headers.h"
// #define _POSIX_C_SOURCE
int ping_function(int pid, int signal_num);
void set_signal_handler();
void handle_eof(int signum);
void set_signals(int signum, void(*handler)(int));

#endif