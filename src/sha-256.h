#ifndef SHA_256_H
#define SHA_256_H

#include <stdlib.h>
#include <stdint.h>

/** The length, in chars, of the digest in string */
#define DGST_LENGTH 65
#define DGST_IN_BYTES 32

int sha256(const uint8_t *msg, size_t messageSize, uint8_t *digest);
void digestToString(const uint8_t *digest, char *dest);
#endif