#include "headers.h"


int comparator(const void *a, const void *b)
{
    file_info **a_1 = (file_info **)a;
    file_info **b_1 = (file_info **)b;

    return (strcmp((*a_1)->name,(*b_1)->name));
}

int peek_function(char** args, int num_arg)
{
    int flag_a=0;
    int flag_l=0;

    for(int i=0;i<num_arg;i++)
    {
        if( strcmp(args[i],"-a") == 0)
        {
            flag_a=1;
        }
        else if( strcmp(args[i],"-l") == 0)
        {
            flag_l=1;
        }
        else if( strcmp(args[i],"-al") == 0 || strcmp(args[i],"-la") == 0)
        {
            flag_a=1;
            flag_l=1;
        }
        else if(strncmp(args[i],"-",1 ) == 0)
        {
            printf("\033[1;31mERROR: Invalid Flags\n");
            printf("\033[0m");
            return 0;
        }
    }
    char* path = (char*)calloc(1024,sizeof(char));
    if( num_arg == 0 || strcmp(args[num_arg-1],"-a") == 0 || strcmp(args[num_arg-1],"-l") == 0 || strcmp(args[num_arg-1],"-al") == 0 || strcmp(args[num_arg-1],"-la") == 0 )
    {
        getcwd(path,1024);
        if(!getcwd(path,1024))
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
        strcpy(path,path_temp);
    }
    else if(strcmp(args[num_arg-1],"..") == 0)
    {
        path=args[num_arg-1];
    }
    else if(strncmp(args[num_arg-1],"~",1) == 0)
    {
        char* path_temp;
        path_temp = displayRoot(1);
        strcpy(path,path_temp);
        memmove(args[num_arg-1],args[num_arg-1]+2,strlen(args[num_arg-1]));
        strcat(path,"/");
        strcat(path,args[num_arg-1]);
    }
    else
    {
        path=args[num_arg-1];       
    }

    DIR *dir;
    struct dirent *dp;
    dir = opendir(path);
    if(!dir)
    {
        printf("\033[1;31mERROR: Couldn't open the directory\n");
        printf("\033[0m");
        return 0;
    }
    double blck_size=0;
    int file_count=0;

    while(dp = readdir(dir))
    {
        file_count++;
    }
    file_info** enteries = (file_info**)malloc(sizeof(file_info*) * file_count);
    // file_info* enteries[file_count];
    for(int i=0;i<file_count;i++)
    {
        enteries[i]=(file_info*)malloc(sizeof(file_info) * 1);
        enteries[i]->permissions= (char*)calloc(1024,sizeof(char));
        enteries[i]->hard_links= (char*)calloc(1024,sizeof(char));
        enteries[i]->owner= (char*)calloc(1024,sizeof(char));
        enteries[i]->group= (char*)calloc(1024,sizeof(char));
        enteries[i]->size= (char*)calloc(1024,sizeof(char));
        enteries[i]->date_mod= (char*)calloc(1024,sizeof(char));
        enteries[i]->name= (char*)calloc(1024,sizeof(char));
    }
    dir = opendir(path);
    if(!dir)
    {
        printf("\033[1;31mERROR: Couldn't open the directory\n");
        printf("\033[0m");
        return 0;
    }
    int pos=0;
    while(dp = readdir(dir))
    {
        struct stat stats;   //https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/stat.html 
        char* stat_path =(char*)calloc(1024,sizeof(char)) ; //https://codeforwin.org/c-programming/c-program-find-file-properties-using-stat-function    
        strcat(stat_path,path);
        strcat(stat_path,"/");
        strcat(stat_path,dp->d_name);

        if(!lstat(stat_path,&stats)) //https://linuxhint.com/lstat-function-c/
        {
            if(dp->d_name[0] == '.' && !flag_a)
            {
                continue;
            }
            blck_size = blck_size + stats.st_blocks;
            int type;   // directory-0, executable-1, rest_files-2
            char* typ;
            if(S_ISDIR(stats.st_mode))  //https://stackoverflow.com/questions/4989431/how-to-use-s-isreg-and-s-isdir-posix-macros
            {
                enteries[pos]->find_type = 0;
                typ="d";
            }
            else if((stats.st_mode & S_IXUSR))
            {
                enteries[pos]->find_type = 1;
                typ="-";
            }
            else
            {
                enteries[pos]->find_type = 2;
                typ="-";
            }
            strcpy(enteries[pos]->name,dp->d_name);
            sprintf(enteries[pos]->hard_links,"%lu", stats.st_nlink);   //https://stackoverflow.com/questions/1401526/how-to-use-printf-to-display-off-t-nlink-t-size-t-and-other-special-types
            strcpy(enteries[pos]->owner,getpwuid(stats.st_uid)->pw_name);   //https://pubs.opengroup.org/onlinepubs/009695399/functions/getpwuid.html
            strcpy(enteries[pos]->group,getpwuid(stats.st_gid)->pw_name);
            sprintf(enteries[pos]->size,"%lld",stats.st_size);
            strftime(enteries[pos]->date_mod, 20, "%b %d %H:%M", localtime(&(stats.st_ctime)));

            char* temp;

            strcat(enteries[pos]->permissions,typ);
            if(stats.st_mode & S_IRUSR) { temp="r";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IWUSR) { temp="w";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IXUSR) { temp="x";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IRGRP) { temp="r";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IWGRP) { temp="w";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IXGRP) { temp="x";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IROTH) { temp="r";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IWOTH) { temp="w";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
            if(stats.st_mode & S_IXOTH) { temp="x";}
            else {temp = "-";}
            strcat(enteries[pos]->permissions,temp);
        }
        else {
            printf("\033[1;31mERROR: Couldn't get information about directory\n");
            printf("\033[0m");
            return 0;
        }
        pos++;
        // free(stat_path);
    }
    qsort(enteries,pos,sizeof(file_info*),comparator);
    if( flag_l==0)
    {
        for(int i=0;i<pos;i++)
        {
            if(enteries[i]->find_type == 0)
            {
                printf("\033[1;34m%s\033[0m\n", enteries[i]->name);
            }
            else if(enteries[i]->find_type == 1)
            {
                printf("\033[1;32m%s\033[0m\n", enteries[i]->name);
            }
            else
            {
                printf("\033[1;37m%s\033[0m\n", enteries[i]->name);                
            }
        }
    }
    else
    {
        printf("total %d\n", (int)ceil(blck_size / 2));
        for(int i=0;i<pos;i++)
        {
            if(enteries[i]->find_type == 0)
            {
                printf("%-7s %-7s %-7s %-7s %-7s %-7s \033[1;34m%s\033[0m\n",enteries[i]->permissions,enteries[i]->hard_links,enteries[i]->owner,enteries[i]->group,enteries[i]->size, enteries[i]->date_mod, enteries[i]->name);
            }
            else if(enteries[i]->find_type == 1)
            {
                printf("%-7s %-7s %-7s %-7s %-7s %-7s \033[1;32m%s\033[0m\n",enteries[i]->permissions,enteries[i]->hard_links,enteries[i]->owner,enteries[i]->group,enteries[i]->size, enteries[i]->date_mod, enteries[i]->name);
            }
            else
            {
                printf("%-7s %-7s %-7s %-7s %-7s %-7s \033[1;37m%s\033[0m\n",enteries[i]->permissions,enteries[i]->hard_links,enteries[i]->owner,enteries[i]->group,enteries[i]->size, enteries[i]->date_mod, enteries[i]->name);
            }
        }
    }

    // free(path);
    for(int i=0;i<file_count;i++)
    {
        free(enteries[i]->permissions);
        free(enteries[i]->hard_links);
        free(enteries[i]->owner);
        free(enteries[i]->group);
        free(enteries[i]->size);
        free(enteries[i]->date_mod);
        free(enteries[i]->name);
        free(enteries[i]);
    }
    free(enteries);
    return 1;
}