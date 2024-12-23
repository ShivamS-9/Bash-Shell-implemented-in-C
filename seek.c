#include "headers.h"

char* search;

int pos_seek;

int comparator_seek(const void *a, const void *b)
{
    info **a_1 = (info **)a;
    info **b_1 = (info **)b;

    return (strcmp((*a_1)->address,(*b_1)->address));
}

int search_function(char* dir_target, int flag_d, int flag_f,info** result)
{
    DIR *dir;
    struct dirent *dp;
    dir = opendir(dir_target);
    if(!dir)
    {
        printf("\033[1;31mERROR: Couldn't open the directory\n");
        printf("\033[0m");
        return 0;
    }

    while(dp = readdir(dir))
    {
        struct stat stats;   //https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/stat.html 
        char* stat_path =(char*)calloc(1024,sizeof(char)) ; //https://codeforwin.org/c-programming/c-program-find-file-properties-using-stat-function    
        strcat(stat_path,dir_target);
        strcat(stat_path,"/");
        strcat(stat_path,dp->d_name);

        if(!lstat(stat_path,&stats)) //https://linuxhint.com/lstat-function-c/
        {
            if(dp->d_name[0] != '.')
            {
                if(S_ISDIR(stats.st_mode))
                {
                    search_function(stat_path,flag_d,flag_f,result);
                    if(flag_d && strcmp(search,dp->d_name) == 0)
                    {
                        strcpy(result[pos_seek]->address,stat_path);
                        result[pos_seek++]->type=1;
                    }
                }
                else
                {
                    if(flag_f && (strncmp(search,dp->d_name,strlen(search)) == 0 && (dp->d_name[strlen(search)] == '.' || dp->d_name[strlen(search)] == '\0')))
                    {
                        result[pos_seek]->address=stat_path;
                        result[pos_seek++]->type=0;                               
                    }
                }
                
            }
        }
        else {
            printf("\033[1;31mERROR: Couldn't get information about directory\n");
            printf("\033[0m");
            return 0;
        }
        // free(stat_path);
    }
    return 1;    
}

int seek_function(char** args, int num_arg)
{
    int flag_d = 0;
    int flag_f = 0;
    int flag_e = 0;
    int check = -1;
    pos_seek = 0;

    search = (char*)calloc(1024,sizeof(char));
    char* target = (char*)calloc(1024,sizeof(char));

    info ** result = (info**)malloc(2048*sizeof(info*));
    for(int i=0;i<2048;i++)
    {
        result[i]=(info*)malloc(sizeof(info));
        result[i]->address = (char*)calloc(1024,sizeof(char));
    }

    for(int i=0;i<num_arg;i++)
    {
        if( strcmp(args[i],"-d") ==0)
        {
            flag_d=1;
            continue;
        }
        else if(strcmp(args[i], "-de") == 0 || strcmp(args[i], "-ed") == 0)
        {
            flag_d=1;
            flag_e=1;
            continue;            
        }
        else if( strcmp(args[i],"-f") == 0)
        {
            flag_f=1;
            continue;
        }
        else if(strcmp(args[i], "-fe") == 0 || strcmp(args[i], "-ef") == 0)
        {
            flag_f=1;
            flag_e=1;
            continue;            
        }
        else if( strcmp(args[i],"-e") == 0)
        {
            flag_e=1;
            continue;
        }
        else if(strcmp(args[i], "-def") == 0 || strcmp(args[i],"-df") == 0 || strcmp(args[i],"-fd") == 0)
        {
            flag_f=1;
            flag_d=1;
            continue;            
        }
        else if(strncmp(args[i],"-",1 ) == 0)
        {
            printf("\033[1;31mERROR: Invalid Flags\n");
            printf("\033[0m");
            return 0;
        }
        if( check == -1)
        {
            strcpy(search,args[i]);
            check = i;
        }
    }
    
    if (check == (num_arg-1) ) {
       if(!getcwd(target,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }       
    }
    else if (strncmp(args[num_arg-1],"~",2) == 0)
    {
        char* path_temp;
        path_temp = displayRoot(1);
        strcpy(target,path_temp);
    }
    else if(strcmp(args[num_arg-1],"..") == 0)
    {
        target=args[num_arg-1];
    }
    else if(strncmp(args[num_arg-1],"~",1) == 0)
    {
        char* path_temp;
        path_temp = displayRoot(1);
        strcpy(target,path_temp);
        memmove(args[num_arg-1],args[num_arg-1]+2,strlen(args[num_arg-1]));
        strcat(target,"/");
        strcat(target,args[num_arg-1]);
    }
    else
    {
        strcpy(target,args[num_arg-1]);     
    }
    if(flag_d == 1 && flag_f == 1)
    {
        printf("\033[1;31mERROR: Invalid Flags\n");
        printf("\033[0m");
        return 0;
    }
    if( flag_d == 0 && flag_f == 0)
    {
        if(search_function(target,1,1,result) == 0)
        return 0;
    }
    else {
        if(search_function(target,flag_d,flag_f,result) == 0)
        return 0;
    }
    if( pos_seek == 0)
    {
        printf("No match found!\n");
        return 0;
    }
    char* path_seek=(char*)calloc(1024,sizeof(char));
    qsort(result,pos_seek,sizeof(info*),comparator_seek);
    for(int i=0;i<pos_seek;i++)
    {
        strcpy(path_seek,"\0");
        strcpy(path_seek,".");
        if(strncmp(target,result[i]->address,strlen(target)) == 0)
        {
            strcat(path_seek,result[i]->address+strlen(target));
            if(result[i]->type == 0)
            {
                printf("\033[1;32m%s\033[0m\n", path_seek);
                if(flag_f == 1 && flag_e ==1 && pos_seek == 1)
                {
                    FILE* fp = fopen(result[0]->address,"r");
                    if(fp == NULL)
                    {
                        printf("\033[1;31mERROR: Missing Permissions for task\n");
                        printf("\033[0m");
                        return 0;
                    }
                    char c;
                    c = fgetc(fp);
                    while (c != EOF)
                    {
                        printf ("%c", c);
                        c = fgetc(fp);
                    }
                    printf("\n");
                    fclose(fp);
                    return 1;
                }
            }
            else if(result[i]->type == 1)
            {
                printf("\033[1;34m%s\033[0m\n", path_seek);
                if(flag_d == 1 && flag_e == 1 && pos_seek == 1)
                {
                    char* ppath = (char*)calloc(1024,sizeof(char));
                    getcwd(ppath,1024);
                    set_prev_path(ppath);
                    int temp = chdir(result[0]->address);
                    if(temp == (-1))
                    {
                        printf("\033[1;31mERROR: Missing Permissions for task\n");
                        printf("\033[0m");
                        return 0;             
                    }
                    return 1;
                }
            }          
        }
        else {
            if(result[i]->type == 0)
            {
                printf("\033[1;32m%s\033[0m\n", result[i]->address);
                if(flag_f == 1 && flag_e ==1 && pos_seek == 1)
                {
                    FILE* fp = fopen(result[0]->address,"r");
                    if(fp == NULL)
                    {
                        printf("\033[1;31mERROR: Missing Permissions for task\n");
                        printf("\033[0m");
                        return 0;
                    }
                    char c;
                    c = fgetc(fp);
                    while (c != EOF)
                    {
                        printf ("%c", c);
                        c = fgetc(fp);
                    }
                    printf("\n");
                    fclose(fp);
                    return 1;
                }
            }
            else if(result[i]->type == 1)
            {
                printf("\033[1;34m%s\033[0m\n", result[i]->address);
                if(flag_d == 1 && flag_e == 1 && pos_seek == 1)
                {
                    char* ppath = (char*)calloc(1024,sizeof(char));
                    getcwd(ppath,1024);
                    set_prev_path(ppath);
                    int temp = chdir(result[0]->address);
                    if(temp == (-1))
                    {
                        printf("\033[1;31mERROR: Missing Permissions for task\n");
                        printf("\033[0m");
                        return 0;             
                    }
                    return 1;
                }
            }
        }
    }

    // free(target);
    free(path_seek);
    for(int i=0;i<2048;i++)
    {
        free(result[i]->address);
        free(result[i]);
    }
    free(result);
    return 1;
}