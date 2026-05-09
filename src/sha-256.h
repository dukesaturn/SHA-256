#ifndef SHA_256_H
#define SHA_256_H

#include <stdlib.h>
#include <stdint.h>

int sha256(const uint8_t *msg, size_t messageSize, uint32_t *digest);
void printDigest(uint32_t *digest);
#endif