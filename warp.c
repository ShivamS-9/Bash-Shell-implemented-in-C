#include "headers.h"

int warp_function(char* dir)
{
    char* path_temp =(char*)calloc(1024,sizeof(char));
    char* prevpath = (char*)calloc(1024,sizeof(char));
    if(strncmp(dir,"~",2) == 0)
    {
        path_temp = displayRoot(1);
        if(!getcwd(prevpath,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        if(chdir(path_temp))
        {
            printf("\033[1;31mERROR: Couldn't change the directory\n");
            printf("\033[0m");
            return 0;            
        }
        printf("%s\n",path_temp);
        set_prev_path(prevpath);
        return 1;
    }
    else if(strcmp(dir,"..") == 0)
    {
        if(!getcwd(prevpath,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        if(chdir(".."))
        {
            printf("\033[1;31mERROR: Couldn't change the directory\n");
            printf("\033[0m");
            return 0;
        }
        if(!getcwd(path_temp,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        printf("%s\n",path_temp);
        set_prev_path(prevpath);
        return 1;
    }
    else if(strncmp(dir,"-",1) == 0)
    {
        path_temp=ret_prev_path();
        if(!getcwd(prevpath,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        if(chdir(path_temp))
        {
            printf("\033[1;31mERROR: Couldn't change the directory\n");
            printf("\033[0m");
            return 0;            
        }
        printf("%s\n", path_temp);
        set_prev_path(prevpath);
        return 1;
    }
    else if(strncmp(dir,"~",1) == 0)
    {
        path_temp = displayRoot(1);
        if(!getcwd(prevpath,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        if(chdir(path_temp))
        {
            printf("\033[1;31mERROR: Couldn't change the directory\n");
            printf("\033[0m");
            return 0;            
        }
        memmove(dir,dir+2,strlen(dir));
        if(chdir(dir))
        {
            printf("\033[1;31mERROR: Couldn't change the directory\n");
            printf("\033[0m");
            return 0;            
        }
        char* copy = (char*)calloc(512,sizeof(char));
        if(!getcwd(copy,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        printf("%s\n",copy);
        set_prev_path(prevpath);
        return 1;
    }
    else
    {
        if(!getcwd(prevpath,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        if(chdir(dir))
        {
            printf("\033[1;31mERROR: Couldn't change the directory\n");
            printf("\033[0m");
            return 0;            
        }
        if(!getcwd(path_temp,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return 0;
        }
        printf("%s\n",path_temp);       
        set_prev_path(prevpath);
        return 1;
    }
}