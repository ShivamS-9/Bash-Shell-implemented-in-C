#ifndef __SEEK_H
#define __SEEK_H
// System Libraries
// User Defined Libraries
#include "headers.h"

int comparator(const void *a, const void *b);
int seek_function(char** args, int num_arg);
typedef struct search_res
{
    char* address;
    int type;
}info;
int search_function(char* dir_target, int flag_d, int flag_f, info** result);

#endif