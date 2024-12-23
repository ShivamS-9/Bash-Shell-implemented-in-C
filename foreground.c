#include "headers.h"

int foreground_function(char* inst, char** arg,int num)
{
    if(strcmp(arg[0],"echo")==0)
    {
        if(arg[1] != NULL )
        {
            if(arg[1][0] == '"')
            {
                char temp[strlen(arg[1])];
                strcpy(temp,arg[1]+1);
                strcpy(arg[1],temp);
            }
            if(arg[num-2][strlen(arg[num-2])-1] =='"')
            arg[num-2][strlen(arg[num-2])-1]='\0';
        }
    }
    time_t start = time(NULL);
    int pid = fork();
    if(pid<0)
    {
        // printf("ERROR\n");
        perror("Error");
        return 0;
        // exit(NULL);
    }
    else if(pid == 0)
    {
        if(execvp(inst,arg))
        {
            printf("\033[1;31mERROR: Couldn't run the command '%s'\n", inst);
            printf("\033[0m");
            exit(0);           
        }
    }
    else
    {
        store_fg(inst,pid);
        int status;
        waitpid(pid, &status, WUNTRACED);
        // free_fg();
        time_t end = time(NULL);
        long long int time = end-start;
        // store_activities(pid,inst);
        if( time > 2 )
        {
            char* prompt_for =(char*)calloc(1024,sizeof(char));
            sprintf(prompt_for,"%s : %ds", inst,time);
            // printf("%s\n", prompt_for);
            select_prompt(0,prompt_for);
        }
        return 1;
    }
}