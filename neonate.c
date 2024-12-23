#include "headers.h"

int neonate_function(int time)
{
    int pid_neo = fork();
    enableRawMode();
    if(pid_neo<0)
    {
        // printf("ERROR\n");
        perror("Error");
        return 0 ;
    }
    else if(pid_neo == 0)
    {
        char* info= (char*)calloc(1024,sizeof(char));
        while(1)
        {
            char* path_stat="/proc/loadavg";    // https://stackoverflow.com/questions/11987495/what-do-the-numbers-in-proc-loadavg-mean-on-linux
            FILE *fp = fopen(path_stat,"r");
            if(fp == NULL)
            {
                printf("\033[1;31mERROR: Couldn't open the file\n");
                printf("\033[0m");
                exit(0);
            }
            fread(info,1,1024,fp);
            char* temp_info = info;
            info = rem_extra_space(info);
            char* pid_str = (char*)calloc(1024,sizeof(char));
            pid_str = __strtok_r(temp_info," ",&temp_info);
            int latest_pid = (-1);      // https://www.baeldung.com/linux/process-id
            while(pid_str != NULL)
            {
                latest_pid = atoi(pid_str);
                pid_str = __strtok_r(temp_info," ",&temp_info);
            }
            printf("%d\n", latest_pid);
            free(pid_str);
            // check_child_status();
            sleep(time);
        }
        free(info);
    }
    else
    {
        // select_prompt(1,'\0');
        // wait(NULL);
        store_fg("neonate -n",pid_neo);
        char c;
        while(1)
        {
            if(read(STDIN_FILENO, &c, 1) == 1 && c == 3)
            {
                printf("^C\n");
                kill(pid_neo,SIGKILL);
                break;
            }
            else if(c == 'x')
            {
                kill(pid_neo,SIGKILL);
                break;
            }
            else if(c == 26)
            {
                char name_neonate[1024];
                // store_activities(fg_proc->pid_proc,fg_proc->name_proc);
                sprintf(name_neonate,"neonate -n %d",time);
                store_activities(pid_neo,name_neonate);
                store_process(pid_neo,"neonate");
                if (kill(pid_neo,SIGSTOP))
                {
                    printf("\033[1;31mERROR: Unable to send the signal\n");
                    printf("\033[0m");
                    return 0;
                }
                // free_fg();
                printf("^Z\n");
                break;
            }
        }
        // free_fg();
    }
    disableRawMode();
    return 1;
}