#include "headers.h"

int proclore_function(int pid_pro)
{
    if(pid_pro == 0)
    {
        pid_pro = getpid();
    }
    char* pid_string = (char*)calloc((ceil(log10(pid_pro))+1),sizeof(char));    //https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c
    sprintf(pid_string,"%d",pid_pro);
    char* path_exec = (char*)calloc(1024,sizeof(char));
    strcpy(path_exec,"/proc/");
    strcat(path_exec,pid_string);
    strcat(path_exec,"/exe");
    char* path_ex = (char*)calloc(1024,sizeof(char));
    char* path_ex1 = (char*)calloc(1024,sizeof(char));
    if(readlink(path_exec,path_ex,1024) == (-1))
    {
        printf("\033[1;31mERROR: Process with %d does not exist\n", pid_pro);
        printf("\033[0m");
        return 0;        
    }        
    strcpy(path_ex1,before(path_ex));
    // int pid_grp_id = getpgid(pid_pro);
    char* path_stat = (char*)calloc(256,sizeof(char));
    strcpy(path_stat,"/proc/");
    strcat(path_stat,pid_string);
    strcat(path_stat,"/stat");      //https://stackoverflow.com/questions/34575285/read-proc-stat-information
    FILE *fp = fopen(path_stat,"r");
    if(fp == NULL)
    {
        printf("\033[1;31mERROR: Couldn't open the file\n");
        printf("\033[0m");
        return 0;
    }
    char* pro_info = (char*)calloc(4096,sizeof(char));
    fread(pro_info,1,4096,fp);
    char* temp = (char*)calloc(512,sizeof(char));
    temp = strtok(pro_info," ");
    char** array = (char**)malloc(52*sizeof(char*));
    for(int i=0;i<52;i++)
    {
        array[i] = (char*)calloc(1024,sizeof(char));
        strcpy(array[i],temp);
        temp=strtok(NULL," ");        
    }
    char check_back ='\0';
    if(atoi(array[4]) == atoi(array[7]))
    {
        check_back='+';        
    }
    printf("pid : %d\n", pid_pro);
    printf("Process Status: %s%c\n", array[2],check_back);
    printf("Process Group : %s\n", array[4]);
    printf("Virtual memory : %s\n", array[22]);
    printf("executable path : %s\n", path_ex1);
    fclose(fp);


    free(pid_string);
    free(path_ex);
    free(path_ex1);
    free(path_exec);
    free(path_stat);
    free(pro_info);
    free(temp);
    for(int i=0;i<52;i++)
    {
        free(array[i]);
    }
    free(array);
    return 1;
}