#include "headers.h"
// #define _POSIX_C_SOURCE
// #include <signal.h>

void set_signals(int signum, void(*handler)(int))
{
    signal(signum,handle_eof);
}

void handle_eof(int signum)
{
    if (signum == SIGQUIT)
    {
        kill_all();
        exit(0);
    }
    else if(signum == SIGINT)
    {
        printf("\n");
        // select_prompt(1,'\0');
    }
    else if(signum ==  SIGTSTP)
    {
        if(fg_proc != NULL)
        {
            int pid = fg_proc->pid_proc;
            struct stat sts;
            char temp_buf[50];
            sprintf(temp_buf,"/proc/%d", pid);
            if (stat(temp_buf, &sts) == -1 && errno == ENOENT) 
            {
                printf("\033[1;31mERROR: No such process is found\n");
                printf("\033[0m");
            }
            store_activities(fg_proc->pid_proc,fg_proc->name_proc);
            store_process(fg_proc->pid_proc,fg_proc->name_proc);
            if (kill(pid,SIGSTOP))
            {
                printf("\033[1;31mERROR: Unable to send the signal\n");
                printf("\033[0m");
            }
            // free_fg();
            printf("\n");         
        }
    }
}

void set_signal_handler()
{
    set_signals(SIGQUIT,handle_eof);
    set_signals(SIGINT,handle_eof);
    set_signals(SIGTSTP,handle_eof);
}

int ping_function(int pid, int signal_num)
{
    struct stat sts;
    char temp_buf[50];
    sprintf(temp_buf,"/proc/%d", pid);
    if (stat(temp_buf, &sts) == -1 && errno == ENOENT) 
    {
        printf("\033[1;31mERROR: No such process is found\n");
        printf("\033[0m");
        return 0;
    }   
    if (kill(pid,signal_num))
    {
        printf("\033[1;31mERROR: Unable to send the signal\n");
        printf("\033[0m");
        return 0;
    }
    return 1;
}