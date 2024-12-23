#ifndef __PEEK_H
#define __PEEK_H
// System Libraries
// User Defined Libraries
#include "headers.h"

int comparator(const void *a, const void *b);
int peek_function(char** args, int num_arg);
typedef struct file    //https://linuxconfig.org/understanding-of-ls-command-with-a-long-listing-format-output-with-permission-bits
{
    char* permissions;
    char * hard_links;
    char* owner;
    char* group;
    char* size;
    char* date_mod;
    char* name;
    int find_type;
}file_info;

#endif