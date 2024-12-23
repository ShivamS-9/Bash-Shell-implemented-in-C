#include "headers.h"

int background_function(char* inst, char** arg,int num) 
{
    // time_t start = time(NULL);
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
    int pid = fork();
    if(pid<0)
    {
        // printf("ERROR\n");
        perror("Error");
        return 0 ;
    }
    else if(pid == 0)
    {
        if(execvp(inst,arg))
        {
            printf("\033[1;31mERROR: Couldn't run the command '%s'\n", inst);
            printf("\033[0m");
            exit(0) ;
        }
        // check_child_status();
    }
    else
    {
        store_process(pid,inst);
        printf("%d\n", pid);
        int j=1;
        strcat(inst, " ");
        while(arg[j] != NULL)
        {
            strcat(inst,arg[j]);
            if(arg[j+1] != NULL)
            {
                strcat(inst," ");            
            }
            j++;
        }
        store_activities(pid,inst);
        return 1;
    }
}