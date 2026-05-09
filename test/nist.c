#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "../src/sha-256.h"
#include <assert.h>

#define BUFF_SIZE 600

void fromHexToByte(const char *src, size_t size, uint8_t *dest);
char *getValueFromRow(char *src);
void readHeader(FILE *f);
void readTest(size_t *len, uint8_t *msg, char *MD, FILE *f);

int main()
{
    /** For SHA256 we have these tests */
    char *filePaths[] = {"test/shavs/SHA256ShortMsg.rsp", "test/shavs/SHA256LongMsg.rsp"};

    for (size_t i = 0; i < 1; i++)
    {
        FILE *rspFile = fopen(filePaths[i], "r");

        if (!rspFile)
        {
            fprintf(stderr, "Error with %s: %s\n", filePaths[0], strerror(errno));
            return 1;
        }

        readHeader(rspFile);
        size_t L = 32;

        for (size_t i = 0; i < L; i++)
        {
            /** Data taken from test */
            size_t len = 0;
            uint8_t msg[BUFF_SIZE] = {0};
            char MD[32] = {0};
            readTest(&len, msg, MD, rspFile);

            uint32_t digest[8] = {0};

            if (!sha256(msg, len / 8, digest))
            {
                printDigest(digest);
                printf("%s\n\n", MD);
            }
            else
            {
                fprintf(stderr, "%s : %s", __FILE_NAME__, strerror(errno));
                fclose(rspFile);
                return 1;
            }
        }
        fclose(rspFile);
    }

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
    char buff[2] = {0};
    size_t j = 0;

    for (size_t i = 0; i < size; i += 2)
    {
        buff[0] = src[i];
        buff[1] = src[i + 1];

        uint8_t tmp = (uint8_t)strtol(buff, NULL, 16);
        dest[j++] = tmp;
    }
}

/**
 * @brief Extract the value in the current row
 *
 * @param src The row
 * @note
 * ```txt
 * Len = 8
 * ```
 * From this row it'll extract `8`
 */
char *getValueFromRow(char *src)
{
    char *tokPtr = strtok(src, "=");
    tokPtr = strtok(NULL, " ");

    return tokPtr;
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
 * At the 7th row we have
 *
 * ```txt
 * [L = 32]
 * ```
 * That represents the amount of the tests.
 */
void readHeader(FILE *f)
{
    char buffer[BUFF_SIZE] = {0};
    for (size_t i = 0; i < 7; i++)
    {
        fgets(buffer, BUFF_SIZE, f);
        if (buffer[0] == '#' && i < 4)
        {
            printf("%s", buffer);
        }
    }
    printf("\n");
}
/**
 * @brief Read a test from file
 *
 * @param len Len
 * @param msg Msg
 * @param MD MD
 * @param f The file
 *
 * A test is composed by this structure
 *
 * ```txt
 * Len = 8
 * Msg = d3
 * MD = 28969cdfa74a12c82f3bad960b0b000aca2ac329deea5c2328ebc6f2ba9802c1
 * ```
 */
void readTest(size_t *len, uint8_t *msg, char *MD, FILE *f)
{
    char buffer[BUFF_SIZE] = {0};
    char *tokPtr = NULL;

    for (size_t i = 0; i < 3; i++)
    {
        fgets(buffer, BUFF_SIZE, f);
        tokPtr = getValueFromRow(buffer);

        switch (i)
        {
        case 0:
            *len = strtol(tokPtr, NULL, 10);
            continue;
        case 1:
            /** In the test `msg` is expressed in hex*/
            fromHexToByte(tokPtr, *len / 8, msg);
            continue;
        case 2:
            strncpy(MD, tokPtr, 64);
        }
    }
    fgets(buffer, BUFF_SIZE, f);
}