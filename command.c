#include "command.h"

char*
command_get(char* input, int length)
{
    char* ret;
    ret = fgets(input, length, stdin);

    if(input[strlen(input) - 1] != '\n')
    {
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    }

    return ret;
}

void
command_interpret(char* input)
{
    int i;
    char *argv[21] = {NULL};

    command_split(input, argv);

    if(argv[0] == NULL)
        return;

    memory_save(argv);

    if(!builtins_interpret(argv))
    {
        for(i = 0; argv[i] != NULL; ++i)
        {
#ifdef DEBUG
            fprintf(stderr, "DEBUG - ARGUMENT %d: %s\n", i, argv[i]);
#endif
        }

        int fork_to_background = 0;
        if(i != 0 && !strcmp(argv[i - 1], "&"))
        {
            fork_to_background = 1;
            argv[i - 1] = NULL;
        }

        int current_pid = safefork();
        if(current_pid == 0)
        {
            char *path = getenv("PATH"), *path_element, attempted_filename[121];

            path_element = strtok(path, ":");

            while(path_element != NULL)
            {
                snprintf(attempted_filename, sizeof(attempted_filename), "%s/%s", path_element, argv[0]);
                execve(attempted_filename, argv, NULL);

                path_element = strtok(NULL, ":");
            }

            printf("ifish: %s: command not found\n", argv[0]);
            exit(-1);
        }
        else if(!fork_to_background)
        {
            int status;

            waitpid(current_pid, &status, 0);
        }
        else
        {
            printf("[%d] %s\n", current_pid, argv[0]);
        }
    }

    for(i = 0; argv[i] != NULL; ++i)
        free(argv[i]);
}

void
command_split(char* input, char* argv[])
{
    char *input_element;
    int i = 0;

    input_element = strtok(input, " \n");

    while(input_element != NULL && i < 21)
    {
        argv[i] = malloc(strlen(input_element) + 1);
        strcpy(argv[i], input_element);

        input_element = strtok(NULL, " \n");
        ++i;
    }
}
