#include <stdio.h>

#include "ifish.h"
#include "prompt.h"
#include "command.h"

int
main()
{
    do
    {
        char input[121];

        prompt_show();

        if(fgets(input, 120, stdin) == NULL)
        {
            printf("\n");
            ifish.quit = 1;
        }
        else
        {
#ifdef DEBUG
            printf("%s", (strcmp(input, "\n") ? input : ""));
#endif
            command_interpret(input);
        }

    } while(!ifish.quit);

    return 0;
}
