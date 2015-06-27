#include <stdio.h>
#include "tools/glt_tools.h"

int main(int argc, char *argv[])
{
    if(!glt_init())
    {
        printf("Failure!!!\n");
    }
    return 0;
}
