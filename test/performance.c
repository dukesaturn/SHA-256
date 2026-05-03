#include "../src/sha-256.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    clock_t start, end;

    int blockSizePow[] = {4, 6, 10, 13, 14};
    int blockSize = 0;
    char *randomData = NULL;
    double currentTimeExecution;
    int counter = 0;

    char result[65] = {0};

    for (int i = 0; i < sizeof(blockSizePow) / sizeof(int); i++)
    {
        blockSize = (int)pow(2, blockSizePow[i]);
        randomData = (char *)malloc(blockSize);
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
                printf("Doing sha256 ops for 3s on %d size blocks: %d sha256 ops in 2.98s\n", blockSize, counter);
                break;
            }
        }

        free(randomData);
    }
}