#include "headers.h"

activity** list = NULL;
int num_of_proc = 0 ;

int comparator_1(const void *a, const void *b)
{
    activity **a_1 = (activity **)a;
    activity **b_1 = (activity **)b;

    return (((*a_1)->pid_proc-(*b_1)->pid_proc));
}

void store_activities(int pid, char* name)
{
    if(list == NULL)
    {
        list = (activity**)malloc(sizeof(activity*) * 1024);
        for(int i = 0 ; i < 1024 ; i++)
        {
            list[i] = (activity*)malloc(sizeof(activity));
            list[i]->pid_proc=__INT_MAX__;
        }
    }
    list[num_of_proc]->name_proc=(char*)calloc(1024,sizeof(char));
    strcpy(list[num_of_proc]->name_proc,name);
    list[num_of_proc]->pid_proc=pid;
    num_of_proc++;

}
int activities_function()
{
    qsort(list,num_of_proc,sizeof(activity*),comparator_1);
    for(int i=0;i<num_of_proc;i++)
    {
        int pid_pro = list[i]->pid_proc;

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
            // printf("\033[1;31mERROR: Process with %d does not exist\n", pid_pro);
            // printf("\033[0m");
            // return 0;   
            continue;     
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
            // printf("\033[1;31mERROR: Couldn't open the file\n");
            // printf("\033[0m");
            // return 0;
            continue;
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
        if(strcmp(array[2],"R") == 0 || strcmp(array[2],"S") == 0)
        {
            printf("%d : %s - Running\n", pid_pro,list[i]->name_proc);
        }
        else if(strcmp(array[2],"T") == 0)
        {
            printf("%d : %s - Stopped\n", pid_pro,list[i]->name_proc);
        }

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

    }
    return 1;
}

void kill_all()
{
    for(int i = 0;i<num_of_proc;i++)
    {
        struct stat sts;
        char temp_buf[50];
        sprintf(temp_buf,"/proc/%d", list[i]->pid_proc);
        if (stat(temp_buf, &sts) == -1 && errno == ENOENT) 
        {
            continue;
        }   
        if (kill(list[i]->pid_proc,9))
        {
            continue;
        }
    }
}
