#include "headers.h"
char* home=NULL;
char* path=NULL;
char* prev_path=NULL;

char* before(char* bf_path)
{
    if(strncmp(home,bf_path,strlen(home)) != 0)
    {
        return bf_path;
    }
    char * dis_path = (char*)calloc(1024,sizeof(char));
    strcat(dis_path,"~");
    strcat(dis_path,bf_path+strlen(home));
    return dis_path;
}
char* displayRoot(int invoke)
{
    if(home == NULL)
    {
        home = (char*)calloc(1024, sizeof(char));
        char * path = (char*)malloc(1024 * sizeof(char));
        if(!getcwd(path,1024))
        {
            printf("\033[1;31mERROR: Couldn't get the current directory\n");
            printf("\033[0m");
            return "\0";
        }
        home = path;
    }
    if(invoke == 1)
    {
        return home;        
    }
    // printf("%s1\n", home);
    if(path == NULL)
    {
        path = (char*)calloc(1024,sizeof(char));
    }
    if(!getcwd(path,1024))
    {
        printf("\033[1;31mERROR: Couldn't get the current directory\n");
        printf("\033[0m");
        return "\0";
    }
    if(strncmp(home,path,strlen(home)) != 0)
    {
        return path;
    }
    return (before(path));
}

void set_prev_path(char* s)
{
    if(prev_path == NULL)
    {
        prev_path = (char*)calloc(1024,sizeof(char));
    }
    strcpy(prev_path,s);
    prev_path[strlen(s)]='\0';
}

char* ret_prev_path()
{
    return prev_path;
}
