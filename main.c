/*
 * Copyright         : 2026 Dks
 * Description       : Didactic implementation of sha-256
 */

#include <stdio.h>
#include "src/sha-256.h"
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        puts("Usage: sha <string>");
        return 1;
    }

    const char* input =  argv[1];
    char result[65] = {0};

    if (!sha256(input,  strlen(input), result))
        puts((char*)result);
    
    return 0;
}