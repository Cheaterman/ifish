#include "command.h"

void
command_interpret(char* input)
{
    char *argv[21];
    command_split(input, argv);

    if(argv[0] == NULL)
        return;

#ifdef DEBUG
    int i;
    for(i = 0; argv[i] != NULL; ++i)
        printf("%s\n", argv[i]);
#endif

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

        printf("\nifish: %s: command not found\n", argv[0]);
        exit(-1);
    }
    else
        wait();
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
