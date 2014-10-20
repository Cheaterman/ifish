#ifndef IFISH_MEMORY_H
#define IFISH_MEMORY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    struct Command* next;
    char* data[15];
} Command;

#include "ifish.h"

void
memory_save(char** argv);

void
memory_erase_last(void);

int
memory_available(int size);

char*
memory_next_available(void);

void
memory_store(char* command);

void
memory_get(Command* command, char* line);

#endif
