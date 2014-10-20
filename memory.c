#include "memory.h"

void
memory_save(char* argv[])
{
    int i;
    char full_command[121];

    for(i = 0; argv[i] != NULL; ++i)
        sprintf(full_command, "%s %s", full_command, argv[i]);

    while(!memory_available(strlen(full_command)))
        memory_erase_last();

    memory_store(full_command);
}

void
memory_erase_last(void)
{
    Command *last_command = ifish.command_history, *previous_command = NULL;

    while(last_command->next != NULL)
    {
        previous_command = last_command;
        last_command = (Command*) last_command->next;
    }

    int i, j;
    for(i = 0; i < 15; ++i)
        if(last_command->data[i] != NULL)
            for(j = 0; j < 64; ++j)
                if(last_command->data[i] == ifish.memory[j])
                    ifish.memory_usage[i] = 0;

    if(previous_command != NULL)
    {
        previous_command->next = NULL;
    }
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

    new_command->next = (struct Command*) ifish.command_history;
    ifish.command_history = new_command;

    for(i = 0; i < strlen(command); i += 8)
    {
        next_available = memory_next_available();
        if(next_available == NULL)
        {
            printf("Assertion failed: next_available is NULL at memory.c:81\n");
            exit(-1);
        }

        strncpy(next_available, command + i, 8);
        new_command->data[i] = next_available;

        for(j = 0; j < 64; ++j)
            if(ifish.memory[j] == next_available)
                ifish.memory_usage[j] = 1;
    }
}

void
memory_get(Command* command, char* line)
{
    if(command->data != NULL)
        strcpy(line, command->data[0]);
}
