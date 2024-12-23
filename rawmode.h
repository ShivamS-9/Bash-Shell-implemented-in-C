#ifndef __RAWMODE_H
#define __RAWMODE_H
// System Libraries
// User Defined Libraries
#include "headers.h"
void die(const char *s);
void disableRawMode();
void enableRawMode();
char* set_raw();
#endif