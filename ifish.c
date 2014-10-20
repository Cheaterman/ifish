#include <stdio.h>

#include "ifish.h"
#include "prompt.h"
#include "command.h"

int
main()
{
    int quit = 0;

    do
    {
        char input[121];

        prompt_show();

        if(fgets(input, 120, stdin) == NULL)
            quit = 1;
        else
        {
#ifdef DEBUG
            printf("%s", input);
#endif
            command_interpret(input);
        }

    } while(!quit);
}
