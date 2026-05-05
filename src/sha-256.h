#ifndef SHA_256_H
#define SHA_256_H

#include <stdlib.h>

int sha256(const char *input, size_t messageSize, char *result);

#endif