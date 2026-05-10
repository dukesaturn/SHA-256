#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "../src/sha-256.h"
#include <assert.h>

#define ROW_DATA_OFFSET 6
#define HEADERS_ROWS 7
#define TEST_BLOCK_ROWS 4
#define BUFFER_SIZE 128
#define RETURN_ERROR_AND_CLEAN                                               \
    free(msg);                                                               \
    fprintf(stderr, ":%d:%s : %s", __LINE__, filePaths[i], strerror(errno)); \
    return 1;

#define RETURN_ERROR                                        \
    fprintf(stderr, ":%d : %s", __LINE__, strerror(errno)); \
    return 1;

/**
 * @note The most long Message length, from `SHA256LongMsg.rsp`
 *
 * ```txt
 *   Len = 51200
 *   Msg = 37ebe98ef52bfb240b9ad3691...
 *   MD = 33b6229592ca719e4e46f35b287617fedadd3b7c38be3c8c1c9f446d2d9085b3
 * ```
 */
#define MAX_MSG_LENGTH_IN_RSP 51200 / 8

void fromHexToByte(const char *src, size_t size, uint8_t *dest);
int readHeader(FILE *f);
int readTest(size_t *len, uint8_t *msg, char *MD, FILE *f);

int main(void)
{
    /** For SHA256 we have these tests */
    const char *filePaths[] = {"test/shavs/SHA256ShortMsg.rsp", "test/shavs/SHA256LongMsg.rsp"};

    /** Iterations, read by the tests */
    size_t L = 32;

    size_t len = 0;
    uint8_t *msg = (uint8_t *)malloc(MAX_MSG_LENGTH_IN_RSP);

    if (!msg)
    {
        RETURN_ERROR;
    }

    char MD[DGST_LENGTH] = {0};

    uint32_t digest[8] = {0};
    char digestHex[DGST_LENGTH] = {0};

    for (size_t i = 0; i < 2; i++)
    {
        FILE *rspFile = fopen(filePaths[i], "r");

        if (!rspFile)
        {
            RETURN_ERROR_AND_CLEAN;
        }

        if (readHeader(rspFile))
        {
            fclose(rspFile);
            RETURN_ERROR_AND_CLEAN;
        }

        for (size_t j = 0; j < L; j++)
        {
            memset(msg, 0, MAX_MSG_LENGTH_IN_RSP);
            /** Data taken from test
             *
             * @note Len is expressed in bits, but, in the implementation, the parameter is the amount of byte.
             * So, in this case, we'll pass
             *
             * ```c
             * len / 8;
             *
             *```
             */
            if (readTest(&len, msg, MD, rspFile))
            {
                fclose(rspFile);
                RETURN_ERROR_AND_CLEAN;
            }

            if (sha256(msg, len / 8, digest))
            {
                fclose(rspFile);
                RETURN_ERROR_AND_CLEAN;
            }

            digestToString(digest, digestHex);
            printf("Test n.[%ld] \nDG: %s \nMD: %s\n", j, digestHex, MD);
            assert(hashCompare256(digestHex, MD));
        }
        fclose(rspFile);
    }

    free(msg);

    return 0;
}

/**
 * @brief Convert a string in hex to a byte, represented by a `uint8_t`
 *
 * @param src The hex string
 * @param size size
 * @param dest the output
 */
void fromHexToByte(const char *src, size_t size, uint8_t *dest)
{
    size_t j = 0;

    for (size_t i = 0; i < size; i++)
    {
        char buff[3] = {0};
        j = i * 2;
        buff[0] = src[j];
        buff[1] = src[j + 1];

        uint8_t tmp = (uint8_t)strtol(buff, NULL, 16);
        dest[i] = tmp;
    }
}

/**
 * @brief Header of the file, the first 7 rows
 *
 * @param f The file
 *
 * ```txt
 * #  CAVS 11.0
 * #  "SHA-256 {Type}Msg" information
 * #  SHA-256 tests are configured for BYTE oriented implementations
 * #  Generated on Tue Mar 15 08:23:38 2011
 * ```
 *
 * @return 0 if it's ok. 1 if an error occurs.
 *
 * At the 7th row we have
 *
 * ```txt
 * [L = 32]
 * ```
 * That represents the amount of the tests.
 *
 * @note Set `errno` to `EIO` if it can't read a row in the header.
 */
int readHeader(FILE *f)
{
    char buffer[BUFFER_SIZE] = {0};

    for (size_t i = 0; i < HEADERS_ROWS; i++)
    {
        if (!fgets(buffer, BUFFER_SIZE, f))
        {
            errno = EIO;
            return 1;
        }

        printf("%s", buffer);
    }
    printf("\n");
    return 0;
}
/**
 * @brief Read a test from file
 *
 * @param len Len
 * @param msg Msg
 * @param MD MD
 * @param f The file
 *
 * @return 0 if it's ok. 1 if an error occurs.
 *
 * @note Set `errno` to `EIO` if it can't read a row in the test.
 *
 * A test is composed by this structure
 *
 * ```txt
 * Len = 8
 * Msg = d3
 * MD = 28969cdfa74a12c82f3bad960b0b000aca2ac329deea5c2328ebc6f2ba9802c1
 * ```
 * 
 * @todo: an implementation with `regex`?
 */
int readTest(size_t *len, uint8_t *msg, char *MD, FILE *f)
{
    char buffer[BUFFER_SIZE] = {0};

    for (size_t i = 0; i < TEST_BLOCK_ROWS; i++)
    {

        if (!fgets(buffer, BUFFER_SIZE, f))
        {
            errno = EIO;
            return 1;
        }

        switch (i)
        {
        case 0:
            *len = (size_t)strtol(buffer + ROW_DATA_OFFSET, NULL, 10);
            break;
        case 1:
            /** In the test `msg` is expressed in hex*/
            fromHexToByte(buffer + ROW_DATA_OFFSET, *len / 8, msg);
            break;
        case 2:
            strncpy(MD, buffer + ROW_DATA_OFFSET - 1, DGST_LENGTH);
            /** To overwrite `\r` char at the end*/
            MD[DGST_LENGTH - 1] = '\0';
            break;
        default:
            break;
        }
    }

    return 0;
}
