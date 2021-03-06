#ifndef IFISH_COMMAND_H
#define IFISH_COMMAND_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "ifish.h"
#include "safefork.h"
#include "memory.h"
#include "builtins.h"

char*
command_get(char*, int);

void
command_interpret(char*);

void
command_split(char* input, char** argv);

#endif
