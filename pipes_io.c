#include "headers.h"
char* rem_extra_space(char* str)
{
    int trail_space=0;      //Remove extra spaces
    while(isspace(str[trail_space]))
    {
        trail_space++;
    }
    int end_space = strlen(str)-1;
    while(end_space >= 0 && isspace(str[end_space]))
    {
        end_space--;
    }
    int command_length = end_space - trail_space + 1;
    char* temp;
    char* final_pi;
    if(command_length < 0)
    {
        temp ='\0';            
    }
    else
    {
        final_pi = (char*)calloc(command_length + 1,sizeof(char));
        for(int j =0;j<command_length;j++)
        {
            final_pi[j] = str[j+trail_space]; 
        }
        final_pi[command_length]='\0';
    }
    return final_pi;
}

int check_in_redir(char* str)
{
    char * temp = str;
    while(*temp != '\0')
    {
        if(*temp == '<')
        {
            return 1;
        }
        temp++;
    }
    return 0;
}

int check_out_redir(char* str)
{
    char * temp = str;
    while(*temp!= '\0')
    {
        if(*temp == '>')
        {
            temp++;
            if(*temp == '>')
            {
                return 2;
            }
            else
            {
                return 1;
            }
        }
        temp++;
    }
    return 0;
}

void pipe_handling(char** command_pi,int num_com)
{
    int fds[2][2];
    int flag=0;
    int out_red_flag;
    int in_red_flag;
    for(int i=0;i<num_com;i++)        // Read-0 Write-1
    {
        if(pipe(fds[i%2]) == (-1))  // https://linuxhint.com/using_pipe_function_c_language/
        {                           // https://www.geeksforgeeks.org/pipe-system-call/
            printf("\033[1;31mERROR: Pipe call not accessed\n");    
            printf("\033[0m");            
        }
        int input_fd=STDIN_FILENO;
        int output_fd=STDOUT_FILENO;
        char * final_pi;
        final_pi=rem_extra_space(command_pi[i]);

        // ONLY PIPING NO I/O REDIRECTION
        in_red_flag=check_in_redir(final_pi);
        out_red_flag=check_out_redir(final_pi);
        if(!(in_red_flag) && i)
        {
            input_fd=fds[!(i%2)][0];
            if(flag == 1)
            {
                close(fds[!(i%2)][1]);
            }
        }
        if(!(out_red_flag) && (i+1)<num_com)
        {
            output_fd=fds[i%2][1];
        }
        flag = 0;
        // I/O Redirection

        char* start = final_pi;
        char* end = final_pi;
        while(*end != '\0')
        {
            if(*end == ';')
            {
                break;
            }
            end++;
        }
        char* in_dir = final_pi;
        while(*in_dir != '\0')
        {
            if(*in_dir == '<')
            {
                break;
            }
            in_dir++;
        }
        char* out_dir = final_pi;
        while(*out_dir != '\0')
        {
            if(*out_dir == '>')
            {
                break;
            }
            out_dir++;
        }
        if(*out_dir == '\0')
        {
            out_dir = end;
        }
        if( *out_dir != '\0')
        {
            char output_file_path[1024];
            if(*(++out_dir) == '>')
            {
                out_dir++;
                strncpy(output_file_path,out_dir,(strlen(out_dir)-strlen(end)));
                output_file_path[strlen(out_dir)-strlen(end)]='\0';
                out_dir--;
                out_dir--;
            }
            else
            {
                out_dir--;
                out_dir++;
                strncpy(output_file_path,out_dir,(strlen(out_dir)-strlen(end)));
                output_file_path[strlen(out_dir)-strlen(end)]='\0';
                out_dir--;
            }
            char * path = rem_extra_space(output_file_path);
            if(out_red_flag == 1)
            {
                FILE* fp1 = fopen(path, "w");
                fclose(fp1);
                output_fd=open(path, O_WRONLY | O_CREAT, 0644);
                if( output_fd < 0 )
                {
                    printf("\033[1;31mERROR: I/O redirection failed\n");
                    printf("\033[0m");
                    return ;                     
                }                
            }
            else if(out_red_flag == 2)
            {
                output_fd=open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if( output_fd < 0 )
                {
                    printf("\033[1;31mERROR: I/O redirection failed\n");
                    printf("\033[0m");
                    return ;                     
                }                
            }
        }
        if( *in_dir != '\0')
        {
            char input_file_path[1024];
            in_dir++;
            int length = strlen(in_dir)-strlen(out_dir);
            strncpy(input_file_path,in_dir,length);
            input_file_path[length]='\0';
            in_dir--;
            char * path= rem_extra_space(input_file_path);
            input_fd=open(path, O_RDONLY, 0644);
            if( input_fd < 0 )
            {
                printf("\033[1;31mERROR: I/O redirection failed\n");
                printf("\033[0m");
                return ;                     
            }                
        }
        if(*in_dir != '\0')
        {
            strncpy(final_pi,start,(strlen(start)-strlen(in_dir)));
            final_pi[(strlen(start)-strlen(in_dir))]='\0';
        }
        else
        {
            strncpy(final_pi,start,(strlen(start)-strlen(out_dir)));
            final_pi[(strlen(start)-strlen(out_dir))] = '\0';
        }
        final_pi=rem_extra_space(final_pi);
        if( final_pi == NULL)
        {
            flag =1;
            continue;            
        }
    
        int restore_input = dup(STDIN_FILENO);      // https://stackoverflow.com/questions/7861611/can-someone-explain-what-dup-in-c-does
        int restore_output = dup(STDOUT_FILENO);    // tried with dup2() but changing anyone of the newfd or oldfd effects the other also

        dup2(input_fd,STDIN_FILENO);                // https://man7.org/linux/man-pages/man2/dup.2.html
        dup2(output_fd,STDOUT_FILENO);

        input_command(final_pi);

        if(input_fd != STDIN_FILENO)
        {
            close(input_fd);    //close() closes a file descriptor, so that it no longer refers to any file and may be reused
        }
        if(output_fd != STDOUT_FILENO)      // http://www.tutorialspoint.com/unix_system_calls/close.htm
        {
            close(output_fd);
        }
        dup2(restore_output,STDOUT_FILENO);
        dup2(restore_input,STDIN_FILENO);
        close(restore_input);       // on not closing leads to creation of file even on deletion, have to remove from background using top and kill
        close(restore_output);
        if(*end != 0)
        {
            end++;
            end = rem_extra_space(end);
            input_command(end);
        }
        free(final_pi);
    }        
}

void job_handling(char* input_io)
{
    char* copy_pi;
    copy_pi = rem_extra_space(input_io);
    char* start = copy_pi;
    while(*start != '\0')
    {
        if(*start == '|')
        {
            start++;
            if(*start == '\0')
            {
                printf("\033[1;31mERROR: Invalid use of pipes\n");
                printf("\033[0m");
                return;
            }
        }
        start++;
    }
    char* token;
    char* copy_pi1=copy_pi;
    int num_com=0;
    token = __strtok_r(copy_pi1,"|\n",&copy_pi1);
    while(token != NULL)
    {
        // pipe_handling(token);
        token = __strtok_r(copy_pi1,"|\n",&copy_pi1);
        num_com++;        
    }
    char * copy_pi_temp = rem_extra_space(input_io);
    char* copy_pi2=copy_pi_temp;
    token = __strtok_r(copy_pi2,"|\n",&copy_pi2);
    char** com_array = (char**)malloc(num_com * sizeof(char*));
    for(int i=0;i<num_com;i++)
    {
        com_array[i] = (char*)calloc(1024,sizeof(char));
    }
    int i=0;
    while(token != NULL)
    {
        strcpy(com_array[i++],token);
        token = __strtok_r(copy_pi2,"|\n",&copy_pi2);
    }
    pipe_handling(com_array,num_com);
    for(int h=0;h<num_com;h++)
    {
        free(com_array[h]);
    }
    free(com_array);
    free(copy_pi);
    free(copy_pi_temp);
}