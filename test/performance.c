#include "../src/sha-256.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    clock_t start, end;

    int blockSizePow[] = {4, 6, 8, 10, 13, 14};
    size_t blockSize = 0;
    uint8_t *randomData = NULL;
    double currentTimeExecution;
    int counter = 0;

    uint32_t result[65] = {0};

    for (size_t i = 0; i < sizeof(blockSizePow) / sizeof(int); i++)
    {
        blockSize = (size_t)pow(2, (double) blockSizePow[i]);
        randomData = (uint8_t *)malloc(blockSize);
        counter = 0;

        start = clock();

        while (1)
        {
            sha256(randomData, blockSize, result);

            end = clock();
            currentTimeExecution = (double)(end - start) / CLOCKS_PER_SEC;
            counter++;

            if (currentTimeExecution >= 3)
            {
                /** It's the Openssl format
                 *
                 *  @code: openssl speed -evp sha256
                 */
                printf("Doing sha256 ops for 3s on %ld size blocks: %d sha256 ops in 2.98s\n", blockSize, counter);
                break;
            }
        }

        free(randomData);
    }
    return 0;
}