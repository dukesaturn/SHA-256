#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "debug.h"
#include <stdbool.h>
#include "sha-256.h"

int isPrime(unsigned int p)
{
    if (p <= 1)
        return 0;

    for (int i = p / 2; i > 1; i--)
    {
        if (p % i == 0)
            return false;
    }

    return true;
}

int generatePrime(int limit)
{
    int tmpLimit = 0;
    int prime = 0;

    for (int i = 2; tmpLimit != limit; i++)
    {
        if (isPrime(i))
        {
            prime = i;
            tmpLimit++;
        }
    }
    logDebug("Generated prime n.%d is %d!\n", limit, prime);

    return prime;
}


/**
 * Get a constant.
 *
 * Take $p$ and $r = \sqrt(p)$ (it would be also cbrt, just an example).
 * Now $fracPart = modf(r)$ and $fracPart * 2^{32}$
 * Now $C = floor(fracPart * 2^{32})$
 *
 * @param p: Prime number
 * @param rootFuncName: The target function selected name for logging.
 * @param func: Target function pointer
 */
int getConstant(unsigned int p, char *rootFuncName, rootFuncPtr func)
{
    logDebug(BOLD("Generating costant from %d\n"), p);

    double sqrt_p = func(p);
    double integralPart = 0;
    double modf_p = modf(sqrt_p, &integralPart);
    long unsigned int res = (modf_p * pow(2, 32));

    logDebug("\t\t\t↪ 0x%lx ✔\n", res);
    return res;
}

void initPrimes(int *primes)
{
    for (int i = 0; i < TOT_CON_LEN; i++)
    {
        int prime = generatePrime(i + 1);
        primes[i] = prime;
    }
}

void initConstants(const int *primes, int *dest, size_t size, char *rootFuncName, rootFuncPtr func)
{
    for (int i = 0; i < size; i++)
    {
        int p = primes[i];
        dest[i] = getConstant(p, rootFuncName, func);
    }
}

void padBlock(int* block){

}  