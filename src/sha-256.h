#ifndef SHA_256_H
#define SHA_256_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/** The length, in chars, of the digest in string */
#define DGST_LENGTH 65

int sha256(const uint8_t *msg, size_t messageSize, uint32_t *digest);
void digestToString(const uint32_t *digest, char* dest);
bool hashCompare256(const char *str1, const char *str2);
#endif