#ifndef IFISH_IFISH_H
#define IFISH_IFISH_H

#define DEBUG

typedef struct
{
    int command_count;
    int current_pid;
}
ifish_globals;

ifish_globals ifish;

#endif
