#include "headers.h"

int main()
{
    set_signal_handler();
    pastevents_init(0);
    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        select_prompt(1,'\0');
    }
}
