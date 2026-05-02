/*
 * Copyright         : 2026 Dks
 * File Name         : sha-256
 * Description       : Didattic implementation fo sha-256
 */

#ifndef SHA_256_H
#define SHA_256_H
#include <stdio.h>

#define FIRST_CON_LEN 8
#define TOT_CON_LEN 64

typedef double (*rootFuncPtr)(double);

int generatePrime(int limit);
int isPrime(unsigned int p);
int getConstant(unsigned int p, char *rootFuncName, rootFuncPtr func);
void initConstants(const int *primes, int *dest, size_t size, char *rootFuncName, rootFuncPtr func);
void initPrimes(int *primes);
#endif