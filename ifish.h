#ifndef IFISH_IFISH_H
#define IFISH_IFISH_H

#include "memory.h"

typedef struct
{
    int command_count;
    Command *command_history;
    char memory[64][8];
    int memory_usage[64];
    int quit;
}
ifish_globals;

ifish_globals ifish;

#endif
