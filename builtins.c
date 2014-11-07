#include "builtins.h"

int
builtins_interpret(char* argv[])
{
    if(!strcmp(argv[0], "quit")
       || !strcmp(argv[0], "exit"))
    {
        ifish.quit = 1;
    }
    else if(!strcmp(argv[0], "h")
       || !strcmp(argv[0], "history"))
    {
        int num_commands = 0;
        Command* command = ifish.command_history;

        if(command != NULL)
        {
            ++num_commands;

            while(command->next != NULL)
            {
                command = (Command*) command->next;
                ++num_commands;
            }
        }

        if(argv[1])
        {
            int wanted_command = 0;

            if(sscanf(argv[1], "%d", &wanted_command)
               && wanted_command > 0
               && wanted_command <= num_commands)
            {
                int i;
                char line[121] = {'\0'};

                command = ifish.command_history;
                for(i = 0; i < wanted_command; ++i)
                    command = (Command*) command->next;

                memory_get(command, line);
                command_interpret(line);

                return 1;
            }

            if(!strcmp(argv[1], "d")
               && sscanf(argv[2], "%d", &wanted_command)
               && wanted_command > 0
               && wanted_command <= num_commands)
            {
                int i;
                Command* previous_command = NULL;

                command = ifish.command_history;
                for(i = 0; i < wanted_command; ++i)
                {
                    previous_command = command;
                    command = (Command*) command->next;
                }

                memory_erase(command);

                return 1;
            }
        }

        printf("History of the last %d commands:\n", num_commands);

        if(ifish.command_history != NULL)
        {
            int i, remaining_commands;

            remaining_commands = num_commands;
            do
            {
                char line[121] = {'\0'};

                command = ifish.command_history;
                for(i = 0; i < remaining_commands - 1; ++i)
                    command = (Command*) command->next;

                memory_get(command, line);
                printf(" %s%d: %s\n", (remaining_commands > 9 ? "" : " "), remaining_commands, line);
                --remaining_commands;
            } while(remaining_commands > 0);
        }

#ifdef DEBUG
        int i, j, k, l;
        char character;

        fprintf(stderr, "DEBUG - MEMORY USAGE:\n");
        for(i = 0; i < 2; ++i)
        {
            for(j = 0; j < 32; ++j)
                fprintf(stderr, "%d", ifish.memory_usage[32 * i + j]);
            fprintf(stderr, "\n");
        }

        fprintf(stderr, "DEBUG - MEMORY CONTENT:\n");
        for(i = 0; i < 8; ++i)
        {
            for(j = 0; j < 2; ++j)
            {
                fprintf(stderr, "##");

                for(k = 0; k < 4; ++k)
                {
                    for(l = 0; l < 8; ++l)
                    {
                        character = ifish.memory[i * 8 + j * 4 + k][l];

                        if(!character)
                            character = '-';

                        fprintf(stderr, "%c", character);
                    }

                    fprintf(stderr, "##");
                }

                fprintf(stderr, "\n");
            }
        }
#endif
    }
    else if(!strcmp(argv[0], "does_it_pipe?"))
        printf("_u ← this is a pipe.\nPut some grass in, enjoy!\n");
    else
        return 0;

    return 1;
}
