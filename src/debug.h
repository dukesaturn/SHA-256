#ifndef MHX_DEBUG_H
#define MHX_DEBUG_H

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <byteswap.h>
#include <ctype.h>

#define BOLD(S) "\033[1m" S "\033[0m"
#define ITALIC(S) "\033[3m" S "\033[0m"

#define RESET "\033[0m"

void getFormattedTime(char *buf, size_t len);
void logMessage(char *level, char *format, ...);
void printBytesFrom32Bit(int blockIndex, uint32_t src);
void printBlock(uint32_t *block, int nOfBlock);
void printLogo();
char checkChar(char c);

/**
 * function-like macros enabled by DEBUG compiling flag.
 */
#ifdef DEBUG
#define logDebug(T, ...) logMessage("DBG", T, ##__VA_ARGS__)
#define phase(T, ...)                      \
    do                                     \
    {                                      \
        puts("Click ENTER to continue.."); \
        getchar();                         \
        logDebug(BOLD(T), ##__VA_ARGS__);  \
        fflush(stdout);                    \
    } while (0)

#define logo() printLogo()
#else
#define logDebug(T, ...)
#define phase(T, ...)
#define logo()
#endif

#endif