#ifndef SHA_256_H
#define SHA_256_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "debug.h"

#define H_LEN 8
#define K_LEN 64
#define BLOCK_SIZE_BYTES 64
#define PRIMES_LEN 64
#define ROUNDS 64
#define W_LEN 16
#define EXP_W_LEN 64
#define UINT32_BITS 32
#define UCHAR_BITS 8
#define BYTE_IN_BITS 8

typedef double (*rootFuncPtr)(double);

int sha256(const char *input, long int originalSize, char *result);

#endif