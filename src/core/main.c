#include <stdio.h>
#include "tools/glt_tools.h"
#include "core/game.h"
// #define DEBUG_FRAMERATE


int main(int argc, char *argv[])
{
    if(!glt_init())
    {
        printf("Failure!!!\n");
        return -1;
    }

    gm_start();

    glt_shutdown();

    return 0;
}

