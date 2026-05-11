#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "../src/sha-256.h"

#define ROW_DATA_OFFSET 6
#define HEADERS_ROWS 7
#define TEST_BLOCK_ROWS 4
#define BUFFER_SIZE 128
#define RETURN_ERROR                                          \
    fprintf(stderr, ":%d : %s\n", __LINE__, strerror(errno)); \
    return 1;

#define RETURN_ERROR_AND_CLEAN \
    free(msg);                 \
    RETURN_ERROR

#define RETURN_ERROR_AND_CLEAN_ALL \
    free(buffer);                  \
    RETURN_ERROR_AND_CLEAN

#define RETURN_ERROR_CLEAN_AND_CLOSE_FILE \
    fclose(rspFile);                      \
    RETURN_ERROR_AND_CLEAN_ALL

#define LENGTH_IN_BYTE(l) l / 8
#define TESTS_AMOUNT 64

/**
 * @note The most long Message length, from `SHA256LongMsg.rsp`
 *
 * ```txt
 *   Len = 51200
 *   Msg = 37ebe98ef52bfb240b9ad3691...
 *   MD = 33b6229592ca719e4e46f35b287617fedadd3b7c38be3c8c1c9f446d2d9085b3
 * ```
 *
 * Parsed by hex the max message is composed by
 *
 * + 51200 / 4 (2 byte for 8 bits)
 *
 * + 9 (6 "Msg = " + 2 "\r\n" + 1 "\0")
 */
#define MAX_MSG_LENGTH_IN_HEX (51200 / 4) + 9
#define MAX_MSG_LENGTH_IN_BYTE 51200 / 8

void fromHexToByte(const char *src, size_t size, uint8_t *dest);
int skipHeader(FILE *f);
int readTest(size_t *len, uint8_t *msg, char *MD, FILE *f, char *buffer);

int main(void)
{
    /** For SHA256 we have three tests: ShortMsg, LongMsg and Monte Carlo. */
    const char *filePaths[] = {
        "test/shavs/SHA256ShortMsg.rsp",
        "test/shavs/SHA256LongMsg.rsp"};

    size_t len = 0;
    uint8_t *msg = (uint8_t *)malloc(MAX_MSG_LENGTH_IN_BYTE);

    if (!msg)
    {
        RETURN_ERROR;
    }

    char MD[DGST_LENGTH] = {0};

    uint32_t digest[8] = {0};
    char digestHex[DGST_LENGTH] = {0};

    char *buffer = (char *)malloc(MAX_MSG_LENGTH_IN_HEX);

    if (!buffer)
    {
        RETURN_ERROR_AND_CLEAN;
    }

    for (size_t i = 0; i < 2; i++)
    {
        FILE *rspFile = fopen(filePaths[i], "r");

        if (!rspFile)
        {
            RETURN_ERROR_AND_CLEAN_ALL;
        }

        if (skipHeader(rspFile))
        {
            RETURN_ERROR_CLEAN_AND_CLOSE_FILE;
        }

        for (size_t j = 0; j < TESTS_AMOUNT; j++)
        {
            memset(msg, 0, MAX_MSG_LENGTH_IN_BYTE);
            /** Data taken from test
             *
             * @note Len is expressed in bits, but, in the implementation, the parameter is the amount of bytes.
             * So, in this case, we'll pass
             *
             * ```c
             * len / 8;
             *
             *```
             */
            if (readTest(&len, msg, MD, rspFile, buffer))
            {
                RETURN_ERROR_CLEAN_AND_CLOSE_FILE;
            }

            if (sha256(msg, LENGTH_IN_BYTE(len), digest))
            {
                RETURN_ERROR_CLEAN_AND_CLOSE_FILE;
            }

            digestToString(digest, digestHex);
            printf("Test n.[%ld] \t%s\t", j + (TESTS_AMOUNT * i), MD);

            if (strcmp(digestHex, MD))
            {
                puts("FAILED");
                puts(digestHex);
                RETURN_ERROR_CLEAN_AND_CLOSE_FILE;
            }
            puts("PASS");
        }
        fclose(rspFile);
    }

    free(msg);
    free(buffer);

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
 * @brief Skip the Header of the file, the first 7 rows
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
 * @note Set `errno` to `EIO` if it can't read a row in the header.
 */
int skipHeader(FILE *f)
{
    char buffer[BUFFER_SIZE] = {0};

    for (size_t i = 0; i < HEADERS_ROWS; i++)
    {
        if (!fgets(buffer, BUFFER_SIZE, f))
        {
            errno = EIO;
            return 1;
        }
    }

    return 0;
}
/**
 * @brief Read a test from file
 *
 * @param len Len
 * @param msg Msg
 * @param MD MD
 * @param f The file
 * @param buffer The buffer in which the test informations are read. The length must be long enough to read the Msg line.
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
int readTest(size_t *len, uint8_t *msg, char *MD, FILE *f, char *buffer)
{

    for (size_t i = 0; i < TEST_BLOCK_ROWS; i++)
    {
        memset(buffer, 0, MAX_MSG_LENGTH_IN_HEX);
        if (!fgets(buffer, MAX_MSG_LENGTH_IN_HEX, f))
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
            fromHexToByte(buffer + ROW_DATA_OFFSET, LENGTH_IN_BYTE(*len), msg);
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
