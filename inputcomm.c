#include "headers.h"

void input_command (char* input_1) {
    char* command;
    char* copy= (char*)calloc(1024,sizeof(char));
    if( input_1[strlen(input_1) - 1] == ' ')
    {
        input_1[strlen(input_1) - 1]='\0';        
    }
    strcpy(copy,input_1);
    char* copy_1 = copy;  
    command = strtok_r(copy_1," \t\n",&copy_1);
    
    int error_flag = 0;
    int flag_2 =0;

    while(command != NULL)
    {
        if(strcmp(command,";") == 0 || strcmp(command,"&") == 0)
        {
            command = strtok_r(copy_1," \t\n",&copy_1);
            continue;
        }


        error_flag = 0;
        int flag=0;

        if( strcmp(command,"warp") == 0)
        {
            // strcat(past_command,command);
            command = strtok_r(copy_1," \t\n",&copy_1);

            if( command == NULL || strcmp(command,";") == 0)
            {
                error_flag = warp_function("~");
            }
            while( command != NULL )
            {
                if( strcmp(command,";") == 0)
                {
                    command = strtok_r(copy_1," \t\n",&copy_1);
                    break;
                }
                error_flag = warp_function(command); 
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
        }

        else if( strcmp(command,"proclore") == 0)
        {
            // strcat(past_command,command);
            command = strtok_r(copy_1," \t\n",&copy_1);

            if( command == NULL || strcmp(command,";") == 0)
            {
                error_flag = proclore_function(0);
                // command = strtok_r(copy_1," \t\n",&copy_1);
            }
            else
            {
                error_flag = proclore_function(atoi(command));
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
        }

        else if(strcmp(command,"ping") == 0)
        {
            command = strtok_r(copy_1," \t\n",&copy_1);

            if(command == NULL || strcmp(command,";") == 0)
            {
                printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                printf("\033[0m");
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            else
            {
                int pid_proc = atoi(command);
                command = strtok_r(copy_1," \t\n",&copy_1);
                if(command == NULL || strcmp(command,";") == 0)
                {
                    printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                    printf("\033[0m");
                    command = strtok_r(copy_1," \t\n",&copy_1);                    
                }
                else
                {
                    int sig_num = atoi(command);
                    error_flag = ping_function(pid_proc,(sig_num%32));
                    command = strtok_r(copy_1," \t\n",&copy_1);                    
                }
            }
        }

        else if( strcmp(command,"fg") == 0)
        {
            command = strtok_r(copy_1," \t\n",&copy_1);

            if(command == NULL || strcmp(command,";") == 0)
            {
                printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                printf("\033[0m");
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            else
            {
                int pid_proc = atoi(command);
                fg_function(pid_proc);
                command = strtok_r(copy_1," \t\n",&copy_1);                
            }            
        }

        else if( strcmp(command,"bg") == 0)
        {
            command = strtok_r(copy_1," \t\n",&copy_1);

            if(command == NULL || strcmp(command,";") == 0)
            {
                printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                printf("\033[0m");
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            else
            {
                int pid_proc = atoi(command);
                bg_function(pid_proc);
                command = strtok_r(copy_1," \t\n",&copy_1);                
            }            
        }

        else if(strcmp(command,"neonate") == 0)
        {
            command = strtok_r(copy_1," \t\n",&copy_1);

            if(command == NULL || strcmp(command,";") == 0 || strcmp(command,"-n") != 0)
            {
                printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                printf("\033[0m");
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            else
            {
                command = strtok_r(copy_1," \t\n",&copy_1);
                int gh=0;
                int check_int=0;
                while(command[gh++] != '\0')
                {
                    if(command[gh] == '.')
                    {
                        check_int = 1;
                        break;
                    }
                }
                if(command == NULL || strcmp(command,";") == 0 || atoi(command) < 0 ||check_int )
                {
                    printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                    printf("\033[0m");
                    command = strtok_r(copy_1," \t\n",&copy_1);                    
                }
                else
                {
                    neonate_function(atoi(command));      
                    command = strtok_r(copy_1," \t\n",&copy_1);                    
                }
            }            
        }
        
        else if(strcmp(command,"iMan") == 0)
        {
            command = strtok_r(copy_1," \t\n",&copy_1);

            if(command == NULL || strcmp(command,";") == 0)
            {
                printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                printf("\033[0m");
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            else
            {
                man_function(command);
                command = strtok_r(copy_1," \t\n",&copy_1);                
            }            
        }

        else if(strcmp(command,"activities") == 0)
        {
            command = strtok_r(copy_1," \t\n",&copy_1);

            if( command == NULL || strcmp(command,";") == 0)
            {
                error_flag = activities_function();
                command = strtok_r(copy_1," \t\n",&copy_1);
            }        
        }

        else if(strcmp(command,"peek") == 0)
        {
            char** arg= (char**)calloc(1024,sizeof(char*));
            for(int i=0;i<1024;i++)
            {
                arg[i] = (char*)calloc(1024,sizeof(char));
            }  
            int i = 0; 
            command = strtok_r(copy_1," \t\n",&copy_1);
            while(command != NULL && strcmp(command,";") != 0)
            {
                strcpy(arg[i++],command);
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            error_flag = peek_function(arg,i);
            for(int j=0;j<1024;j++)
            {
                free(arg[j]);
            }
            free(arg);              
        }

        else if(strcmp(command,"seek") == 0)
        { 
            char** arg= (char**)calloc(1024,sizeof(char*));
            for(int i=0;i<1024;i++)
            {
                arg[i] = (char*)calloc(1024,sizeof(char));
            }  
            int i = 0; 
            command = strtok_r(copy_1," \t\n",&copy_1);
            while(command != NULL && strcmp(command,";") != 0)
            {
                strcpy(arg[i++],command);
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            error_flag = seek_function(arg,i); 
            for(int j=0;j<1024;j++)
            {
                free(arg[j]);
            }
            free(arg);                       
        }

        else if(strcmp(command,"pastevents") == 0)
        {
            // printf("1\n");
            flag_2=1;
            command = strtok_r(copy_1," \t\n",&copy_1);
            if(command == NULL || strcmp(command,";") == 0 )
            {
                pastevents("\0",0,"\0");
            }
            else if (strcmp(command,"purge") == 0)
            {
                pastevents(command,0,"\0");
                command = strtok_r(copy_1," \t\n",&copy_1);
            }
            else
            {
                printf("\033[1;31mERROR: Couldn't run the command '%s'\n", input_1);
                printf("\033[0m");
                command = strtok_r(copy_1," \t\n",&copy_1);            
            }            
        }
        else
        {
            char** arg= (char**)calloc(1024,sizeof(char*));
            for(int i=0;i<1024;i++)
            {
                arg[i] = (char*)calloc(1024,sizeof(char));
            }
            int i = 0;
            strcpy(arg[i++],command);
            if(arg[i-1][strlen(command)-1] == '&')
            {
                arg[i-1][strlen(command)-1]='\0';
            }
            command = strtok_r(copy_1," \t\n",&copy_1);
            if(command == NULL || strcmp(command,";") == 0)
            {
                arg[i++]=NULL;
                error_flag = foreground_function(arg[0],arg,i);
            }
            else if(strcmp(command,"&") == 0)
            {
                arg[i++]=NULL;
                error_flag = background_function(arg[0],arg,i);
            }
            else if( strcmp(command,"&") != 0 || strcmp(command,";") != 0)
            {
                while( command != NULL && strcmp(command,"&") != 0 && strcmp(command,";") != 0 )
                {
                    strcpy(arg[i++],command);
                    if(arg[i-1][strlen(command)-1] == '&')
                    {
                        arg[i-1][strlen(command)-1]='\0';
                        arg[i++]=NULL;
                        background_function(arg[0],arg,i);
                        command = strtok_r(copy_1," \t\n",&copy_1);
                        flag=1;
                        break;
                    }
                    command = strtok_r(copy_1," \t\n",&copy_1);
                }
                if(flag == 1)
                {continue;}
                if(command == NULL || strcmp(command,";") == 0)
                {
                    arg[i++]=NULL;
                    error_flag = foreground_function(arg[0],arg,i);
                }
                else if(strcmp(command,"&") == 0)
                {
                    arg[i++]=NULL;
                    error_flag = background_function(arg[0],arg,i);
                }
            }
            for(int j=0;j<1024;j++)
            {
                free(arg[j]);
            }
            free(arg); 
        }
        
    }
    // if(strncmp(input_1,"pastevents",strlen("pastevents")) != 0)
    // {
    //     if(flag_2 == 0)
    //     {
    //         if(strlen(input_1) != 0)
    //         {
    //             pastevents_add(input_1);
    //             pastevents_write();
    //         }
    //     }
    // }
    free(command);
    free(copy);
    // check_child_status();
}

void take_input()
{
    char* input;
    // fgets(input, 4096, stdin);
    input = set_raw();
    check_child_status();

    char* copy_2= (char*)calloc(1024,sizeof(char));
    // input[strlen(input) - 1]='\0';
    strcpy(copy_2,input);
    char* copy_3 = copy_2;
    char* command_4; 
    command_4 = strtok_r(copy_2," \t\n",&copy_2);
    int fl = 0;
    int flag_2=0;

    while(command_4 != NULL)
    {
        if(strcmp("pastevents", command_4) == 0)
        {
            flag_2=1;
            command_4 = strtok_r(copy_2," \t\n",&copy_2);
            if(command_4 != NULL && strcmp(command_4,"execute") == 0)
            {
                command_4 = strtok_r(copy_2," \t\n",&copy_2);
                if( command_4 == NULL)
                {
                    printf("\033[1;31mERROR: Couldn't run the command\n");
                    printf("\033[0m");
                    return ;                    
                }
                int check_exec=0;
                for(int i=1;i<16;i++)
                {
                    if(atoi(command_4) == i)
                    {
                        check_exec = 1;
                        break;
                    }
                }
                if(check_exec == 0)
                {
                    printf("\033[1;31mERROR: Couldn't run the command\n");
                    printf("\033[0m");
                    return ;                    
                }
                fl = 1;
                pastevents("execute",atoi(command_4), input);
                break;
            }
            else
            {
                continue;
            }   
            
        }
        command_4 = strtok_r(copy_2," \t\n",&copy_2);
    }
    if(fl == 0)
    {
        if(flag_2 == 0)
        {
            if(strlen(input) != 0)
            {
                pastevents_add(input);
                pastevents_write();
            }
        }
        // input_command(input);
        // int x = strlen(input);
        if(input[strlen(input)-1] == '\n')
        {
            input[strlen(input)-1] = '\0';
        }
        char * co = input;
        char * command_tok=__strtok_r(co,";",&co);
        while(command_tok != NULL)
        {
            job_handling(command_tok);
            command_tok=__strtok_r(co,";",&co);
        }
    }
    free(input);
}