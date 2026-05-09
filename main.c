/*
 * Copyright         : 2026 Dks
 * Description       : Didactic implementation of sha-256
 */

#include <stdio.h>
#include "src/sha-256.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define PROGRAM "sha256"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", PROGRAM);
        return 1;
    }

    const char *input = argv[1];
    char result[65] = {0};

    if (!sha256(input, strlen(input), result))
        puts((char *)result);
    else
    {
        fprintf(stderr, "Error with %s: %s\n", PROGRAM, strerror(errno));
        return 1;
    }

    return 0;
}