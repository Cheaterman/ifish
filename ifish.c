#include <stdio.h>

#include "ifish.h"
#include "prompt.h"
#include "command.h"

int
main()
{
    ifish.command_history = NULL;

    do
    {
        char input[121];

        prompt_show();

        if(command_get(input, sizeof(input)) == NULL)
        {
            printf("\n");
            ifish.quit = 1;
        }
        else
        {
#ifdef DEBUG
            if(strcmp(input, "\n"))
                fprintf(stderr, "DEBUG - INPUT: %s%s", input,
                (
                    input[strlen(input) - 1] != '\n'
                    ? "\n"
                    : ""
                ));
#endif

            command_interpret(input);
        }

    } while(!ifish.quit);

    return 0;
}
