#include "command.h"

void
command_interpret(char* input)
{
    char *argv[21];
    command_split(input, argv);

    if(argv[0] == NULL)
        return;

    if(!strcmp(argv[0], "quit")
       || !strcmp(argv[0], "exit"))
    {
        ifish.quit = 1;
        return;
    }

    if(!strcmp(argv[0], "h"))
    {
        char line[121] = "";
        int num_commands = 0;
        Command* command = ifish.command_history;

        while(command != NULL)
        {
            command = (Command*) command->next;
            ++num_commands;
        }

        printf("History of the last %d commands:\n", num_commands);

        if(ifish.command_history != NULL)
        {
            int i, remaining_commands;

            remaining_commands = num_commands;
            do
            {
                command = ifish.command_history;
                for(i = 0; i < remaining_commands - 1; ++i)
                    command = (Command*) command->next;

                memory_get(command, line);
                printf(" %s%d: %s\n", (remaining_commands >= 9 ? "" : " "), remaining_commands--, line);
            } while(remaining_commands > 0);
        }

#ifdef DEBUG
        int i, j, k, l;

        printf("DEBUG - MEMORY USAGE:\n");
        for(i = 0; i < 2; ++i)
        {
            for(j = 0; j < 32; ++j)
                printf("%d", ifish.memory_usage[32 * i + j]);
            printf("\n");
        }

        printf("DEBUG - MEMORY CONTENT:\n");
        for(i = 0; i < 8; ++i)
        {
            for(j = 0; j < 8; ++j)
            {
                for(k = 0; k < 2; ++k)
                {
                    for(l = 0; l < 4; ++l)
                        printf("%c", ifish.memory[i * 8 + j][k * 4 + l]);

                    printf(" ");
                }

                printf("##");
            }

            printf("\n");
        }

        printf("DEBUG - COMMAND HISTORY:\n");
        Command* command = ifish.command_history;
        do
        {
            printf("Data0: %s\n", command->data[0]);
            command = (Command*) command->next;
        } while(command != NULL);
#endif
        return;
    }

    int i, fork_to_background = 0;
    for(i = 0; argv[i] != NULL; ++i)
    {
#ifdef DEBUG
        printf("%s\n", argv[i]);
#endif
    }

    if(i != 0 && !strcmp(argv[i - 1], "&"))
    {
        fork_to_background = 1;
        argv[i - 1] = NULL;
    }

    memory_save(argv);

    int current_pid = safefork();
    if(current_pid == 0)
    {
        char *path = getenv("PATH"), *path_element, attempted_filename[120];

        path_element = strtok(path, ":");

        while(path_element != NULL)
        {
            sprintf(attempted_filename, "%s/%s", path_element, argv[0]);
            execve(attempted_filename, argv, NULL);

            path_element = strtok(NULL, ":");
        }

        printf("ifish: %s: command not found\n", argv[0]);
        exit(-1);
    }
    else if(!fork_to_background)
    {
        int status;

        waitpid(current_pid, &status, WCONTINUED);

        if(WEXITSTATUS(status) != 255 && ifish.command_count < 10)
            ++ifish.command_count;
    }
    else
    {
        printf("[%d] %s\n", current_pid, argv[0]);
    }
}

void
command_split(char* input, char* argv[])
{
    char *input_element, *word;
    int i = 0;

    input_element = strtok(input, " \n");

    while(input_element != NULL)
    {
        word = malloc(strlen(input_element) + 1);
        sprintf(word, "%s", input_element);
        argv[i++] = word;

        input_element = strtok(NULL, " \n");
    }

    argv[i] = NULL;
}
