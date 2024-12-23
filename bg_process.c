#include "headers.h"

proc* start;
proc* fg_proc = NULL;

void store_process(int pid, char* name)
{
    if(start == NULL)
    {
        start = (proc*)malloc(sizeof(proc));
        start->pid_proc=pid;
        start->name_proc=(char*)calloc(1024,sizeof(char));
        strcpy(start->name_proc,name);
        start->next=NULL;
        start->prev = NULL;
        return;
    } 
    proc* temp = (proc*)malloc(sizeof(proc));
    temp->name_proc=(char*)calloc(1024,sizeof(char));
    strcpy(temp->name_proc,name);
    temp->pid_proc=pid;
    temp->next = start;
    temp->prev = NULL;
    start->prev = temp;
    start=temp;
    // free(temp->name_proc);
    // free(temp); 
    return;
}

void check_child_status()   //https://www.ibm.com/docs/en/ztpf/1.1.0.15?topic=apis-waitpidobtain-status-information-from-child-process
{
    pid_t child_pid;
    int chld_state;
    proc* temp = start;
    proc* before;
    proc* after;
    proc* present;
    int statu_norm;
    while((child_pid = waitpid( -1, &chld_state, WNOHANG)) > 0)
    {
        statu_norm = WIFEXITED(chld_state);    //https://pubs.opengroup.org/onlinepubs/9699919799/functions/waitpid.html
        while(temp != NULL)
        {
            if(temp->pid_proc == child_pid)
            {
                if(statu_norm > 0)
                {
                    // printf("\n");
                    printf("%s exited normally (%d)\n",temp->name_proc,temp->pid_proc);
                    // prompt();
                }
                else
                {
                    printf("%s exited abnormally (%d)\n",temp->name_proc,temp->pid_proc);
                    // printf("\n");
                    // prompt();
                }
                present=temp;
                before=temp->prev;
                after=temp->next;
                if(before == NULL)
                {
                    temp = after;
                }
                else
                {
                    before->next=after;
                }
                if(after == NULL)
                {
                    temp = before;
                }
                else
                {
                    after->prev=before;
                }
                break;
            }
            temp=temp->next;
        }
    }
    // prompt();        
}

void rem_bg_to_fg(int child_pid)
{
    proc* temp = start;
    proc* before;
    proc* after;
    proc* present;
    while(temp != NULL)
    {
        if(temp->pid_proc == child_pid)
        {
            present=temp;
            before=temp->prev;
            after=temp->next;
            if(before == NULL)
            {
                temp = after;
            }
            else
            {
                before->next=after;
            }
            if(after == NULL)
            {
                temp = before;
            }
            else
            {
                after->prev=before;
            }
            break;
        }
        temp=temp->next;
    }

}

char* get_proc_name(int pid)
{
    proc* temp = start;
    proc* before;
    proc* after;
    proc* present;
    while(temp != NULL)
    {
        if(temp->pid_proc == pid)
        {
            return (temp->name_proc);            
        }
        temp=temp->next;
    }

}

void store_fg(char* name, int pid)
{
    if(fg_proc == NULL)
    {
        fg_proc = (proc*)malloc(sizeof(proc));
        fg_proc->pid_proc=pid;
        fg_proc->name_proc=(char*)calloc(1024,sizeof(char));
        strcpy(fg_proc->name_proc,name);
        fg_proc->next=NULL;
        fg_proc->prev = NULL;
        return;
    } 
    else
    {
        strcpy(fg_proc->name_proc,name); 
        fg_proc->name_proc[strlen(name)]='\0';
        fg_proc->pid_proc=pid;
        return;
    }
}

void free_fg()
{
    free(fg_proc->name_proc);
    // free(fg_proc);
    fg_proc=NULL;
}