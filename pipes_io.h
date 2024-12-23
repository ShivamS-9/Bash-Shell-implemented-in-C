#ifndef __PIPES_IO_H
#define __PIPES_IO_H

// System Libraries

// User Defined Libraries

#include "headers.h"

void job_handling(char* input_io);
void pipe_handling(char** command_pi,int num_pipes);
char* rem_extra_space(char* str);
int check_out_redir(char* str);
int check_in_redir(char* str);

#endif