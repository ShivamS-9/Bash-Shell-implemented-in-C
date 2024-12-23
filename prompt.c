#include "headers.h"

void select_prompt(int flag,char* fore)
{
    if (flag == 1 )
    {
        prompt();
    }
    if(flag == 0)
    {
        time_prompt(fore);
    }
}

void prompt() {
    // Do not hardcode the prompt
    char* user;
    char system[256];
    system[256]='\0';
    int hostid;
    user = getlogin();
    hostid = gethostname(system, 256);
    printf("<%s@%s:%s>", user,system,displayRoot(0));
    // check_child_status();
    take_input();
    // printf("<Everything is a file> ");    
}

void time_prompt(char* fore) {
    // Do not hardcode the prompt
    char* user;
    char system[256];
    system[256]='\0';
    int hostid;
    user = getlogin();
    hostid = gethostname(system, 256);
    printf("<%s@%s:%s %s>", user,system,displayRoot(0),fore);
    take_input();
    // printf("<Everything is a file> ");    
}
