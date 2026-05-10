/*
 * Copyright         : 2026 Dks
 * Description       : Didactic implementation of sha-256
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include "src/sha-256.h"

#define PROGRAM "sha256"


/**
 * @note This program is just an example of the algorithm implementation, and handles
 * only string inputs.
 * 
 */
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", PROGRAM);
        return 1;
    }

    const uint8_t *msg = (uint8_t *)argv[1];
    uint32_t digest[8] = {0};
    char digestHex[DGST_LENGTH] = {0};

    if (!sha256(msg, strlen(argv[1]), digest)){
        digestToString(digest, digestHex);
        printf("%s\n", digestHex);
    }
    else
    {
        fprintf(stderr, "Error with %s: %s\n", PROGRAM, strerror(errno));
        return 1;
    }

    return 0;
}