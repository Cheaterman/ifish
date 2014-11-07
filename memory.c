#include "memory.h"

void
memory_save(char* argv[])
{
    int i;
    char full_command[121] = {'\0'};

    for(i = 0; argv[i] != NULL; ++i)
        sprintf(full_command, "%s %s", full_command, argv[i]);

    while(!memory_available(strlen(full_command)))
        memory_erase_last();

    memory_store(full_command);
    ++ifish.command_count;
}

void
memory_erase_last(void)
{
    Command *last_command = ifish.command_history, *previous_command = NULL;

    if(last_command == NULL)
    {
        printf("Assertion failed: last_command is NULL at memory.c:25\n");
        exit(-1);
    }

    while(last_command->next != NULL)
    {
        previous_command = last_command;
        last_command = (Command*) last_command->next;
    }

    int i, j;
    for(i = 0; i < ARRAY_SIZE(last_command->data); ++i)
    {
        if(last_command->data[i] != NULL)
        {
            memset(last_command->data[i], 0, 8);
            for(j = 0; j < 64; ++j)
                if(last_command->data[i] == ifish.memory[j])
                    ifish.memory_usage[j] = 0;
        }
    }

    if(previous_command != NULL)
    {
        previous_command->next = NULL;
    }

    --ifish.command_count;
    free(last_command);
}

int
memory_available(int size)
{
    int i;

    for(i = 0; i < 64; ++i)
        if(!ifish.memory_usage[i])
            size -= 8;

    return (size <= 0);
}

char*
memory_next_available(void)
{
    int i;

    for(i = 0; i < 64; ++i)
        if(!ifish.memory_usage[i])
            return ifish.memory[i];

    return NULL;
}

void
memory_store(char* command)
{
    int i, j;
    char* next_available;
    Command* new_command;
    new_command = malloc(sizeof(Command));

    for(i = 0; i < ARRAY_SIZE(new_command->data); ++i)
        new_command->data[i] = NULL;

    new_command->next = (struct Command*) ifish.command_history;
    ifish.command_history = new_command;

    for(i = 0; i < strlen(command); i += 8)
    {
        next_available = memory_next_available();
        if(next_available == NULL)
        {
            printf("Assertion failed: next_available is NULL at memory.c:94\n");
            exit(-1);
        }

        strncpy(next_available, command + i, 8);
        new_command->data[(int)(i / 8)] = next_available;

        for(j = 0; j < 64; ++j)
            if(ifish.memory[j] == next_available)
                ifish.memory_usage[j] = 1;
    }
}

void
memory_get(Command* command, char* line)
{
    int i;
    char data[9] = {'\0'};

    for(i = 0; i < ARRAY_SIZE(command->data); ++i)
    {
        if(command->data[i] != NULL)
        {
            strncpy(data, command->data[i], 8);
            strncat(line, data, 120);
            line[120] = '\0';
        }
    }
}
