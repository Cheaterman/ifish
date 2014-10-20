#include "prompt.h"

void
prompt_show()
{
    char *user = getenv("USER");

    printf("%s@%s %d $ ", (user != NULL ? user : '\0'), "ifish", ifish.command_count);
}
