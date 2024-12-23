#include "headers.h"

char** hist_array;
int* number_entries;
int* pos;
char* path_hist;

void pastevents_init(int invoke) {
    if( invoke == 0)
    {
        path_hist = (char*)calloc(2048,sizeof(char));
        FILE* fp1 = fopen("history.txt", "a");
        getcwd(path_hist,1024);
        strcat(path_hist,"/history.txt");
        // fprintf(fp1,"%s","\0");
        fclose(fp1);
        hist_array = (char**)calloc(15,sizeof(char*));
        for(int i=0;i<15;i++)
        {
            hist_array[i]=(char*)calloc(2048,sizeof(char));
        }
        number_entries =(int*)calloc(1,sizeof(int));
        pos = (int*)malloc(sizeof(int)*1);
    }
    
    number_entries[0] = 0;
    
    pos[0] =(-1);
    FILE* fp = fopen(path_hist, "r");    //https://www.geeksforgeeks.org/history-command-in-linux-with-examples/
    if(fp == NULL)
    {
        printf("\033[1;31mERROR: Couldn't open the file\n");
        printf("\033[0m");
        return;
    }
    char* temp_line = (char*)calloc(2048,sizeof(char));

    while(fgets(temp_line, 2048, fp)) //https://www.cppbuzz.com/program-to-read-text-file-line-by-line-in-c
    {
        strcpy(hist_array[number_entries[0]],temp_line);
        if(strlen(hist_array[number_entries[0]]) != 0)
        {
            hist_array[number_entries[0]][strlen(hist_array[number_entries[0]])-1] = '\n';
            hist_array[number_entries[0]][strlen(hist_array[number_entries[0]])] = '\0';
        }
        number_entries[0]++;
        pos[0]++; 
    }
    fclose(fp);
    free(temp_line);
}

void pastevents_write() {
    FILE* fp = fopen(path_hist, "w");     //https://www.geeksforgeeks.org/basics-file-handling-c/
    if(fp == NULL)
    {
        printf("\033[1;31mERROR: Couldn't open the file\n");
        printf("\033[0m");
        return;
    }
    if(pos[0] != (-1))
    {
        int i = (pos[0]+1) % 15;
        while( (i != pos[0]))
        {
            // printf("%s\n", hist_array[i]);
            fprintf(fp,"%s",hist_array[i]);
            i = (i+1)%15;
        }
        // printf("%s\n", hist_array[i]);
        fprintf(fp,"%s",hist_array[i]);
    }
    fclose(fp);
}

void pastevents_add(char * command) {

    command[strlen(command)]='\n';
    command[strlen(command)]='\0';
    if( pos[0] != (-1))
    {
        if(strcmp(command,hist_array[pos[0]]) == 0)
        {
            return ;
        }
    }
    pos[0]=(pos[0]+1) % 15;
    strcpy(hist_array[pos[0]],command);
    if(number_entries[0] < 15)
    {
        number_entries[0] = number_entries[0] + 1;
    }    
    return;
}

void pastevents(char* argument, int index,char* input_1_exe)
{
    pastevents_write();
    pastevents_init(1);
    if( strcmp(argument,"purge") == 0)
    {
       for(int i=0;i<number_entries[0];i++)
       {
        strcpy(hist_array[i],"\0");
       }
       pastevents_write();
    }
    else if(strcmp( argument,"execute") == 0)
    {
        if(index > number_entries[0])
        {
            printf("\033[1;31mERROR: Couldn't run the command\n");
            printf("\033[0m");
            return ;
        }
        char * input_past_exec = (char*)calloc(1024,sizeof(char));
        char* temp=input_1_exe;
        char* command_3; 
        char* command_2= (char*)calloc(1024,sizeof(char));  
        command_3 = strtok_r(temp," \t\n",&temp);
        while(command_3 != NULL)
        {
            if(strcmp(command_3,"pastevents") == 0)
            {
                command_3 = strtok_r(temp," \t\n",&temp);
                if(command_3 != NULL && strcmp(command_3,"execute") == 0)
                {
                    command_3 = strtok_r(temp," \t\n",&temp);
                    strcpy(command_2,hist_array[number_entries[0]-atoi(command_3)]);
                    command_2[strlen(command_2)-1]='\0';
                    strcat(input_past_exec,command_2);
                    strcat(input_past_exec," ");
                }
                else 
                {
                    strcat(input_past_exec,"pastevents ");
                    if(command_3 != NULL)
                    {
                        strcat(input_past_exec,command_3);
                        strcat(input_past_exec," ");                    
                    }                        
                }
            }
            else 
            {
                strcat(input_past_exec,command_3);
                strcat(input_past_exec," ");
            }
            command_3 = strtok_r(temp," \t\n",&temp);
        }        
        input_past_exec[strlen(input_past_exec)-1]='\0';
        if(strlen(input_past_exec) != 0)
        {
            pastevents_add(input_past_exec);
            pastevents_write();
        }
        char * co = input_past_exec;
        char * command_tok=__strtok_r(co,";",&co);
        while(command_tok != NULL)
        {
            job_handling(command_tok);
            command_tok=__strtok_r(co,";",&co);
        }
    }
    else
    {
        if(strcmp(hist_array[0],"\0") != 0)
        {
            for(int i=0;i<number_entries[0];i++)
            {
                printf("%s", hist_array[i]);
            }
        }
    }
}
