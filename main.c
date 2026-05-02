/*
 * Copyright         : 2026 Dks
 * File Name         : sha-256
 * Description       : Didattic implementation fo sha-256
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "src/debug.h"
#include "src/sha-256.h"

int main(int argc, char **argv)
{
    if(argc < 2){
        puts("sha <string>");
        exit(EXIT_FAILURE);
    }

    logo();
    char *input = argv[1];
    char result[65] = {0};

    sha256(input, strlen(input), result);
    puts(result);
}