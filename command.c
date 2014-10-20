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
