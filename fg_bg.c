#include "headers.h"

int fg_function(int pid)
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
    char* inst = get_proc_name(pid);
    rem_bg_to_fg(pid);
    signal(SIGTTOU,SIG_IGN);    //https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-tcsetpgrp-set-foreground-process-group-id
    if(tcsetpgrp(STDIN_FILENO,getpid()))
    {
        signal(SIGTTOU,SIG_DFL);
        printf("\033[1;31mERROR: Unable to give process control\n");
        printf("\033[0m");
        return 0;        
    }
    if(kill(pid,SIGCONT))
    {
        printf("\033[1;31mERROR: Unable to process the command\n");
        printf("\033[0m");
        return 0;        
    }
    store_fg(inst,pid);
    time_t start = time(NULL);
    int status;
    waitpid(pid, &status, WUNTRACED);
    // void free_fg();
    time_t end = time(NULL);
    long long int time = end-start;
    // store_activities(pid,inst);
    if(tcsetpgrp(STDIN_FILENO,getpgid(0)))
    {
        printf("\033[1;31mERROR: Unable to give terminal control\n");
        printf("\033[0m");
        return 0;        
    }
    signal(SIGTTOU,SIG_DFL);
    if( time > 2 )
    {
        char* prompt_for =(char*)calloc(1024,sizeof(char));
        sprintf(prompt_for,"%s : %ds", inst,time);
        // printf("%s\n", prompt_for);
        select_prompt(0,prompt_for);
    }
    return 1;
}
int bg_function(int pid)
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
    if (kill(pid,SIGCONT))
    {
        printf("\033[1;31mERROR: Unable to process the command\n");
        printf("\033[0m");
        return 0;
    }
    return 1;
}