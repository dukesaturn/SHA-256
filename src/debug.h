#ifndef MHX_DEBUG_H
#define MHX_DEBUG_H

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#define BOLD(S) "\033[1m" S "\033[0m"
#define ITALIC(S) "\033[3m" S "\033[0m"

#define RESET "\033[0m"

void getFormattedTime(char *buf, size_t len);
void logMessage(char *level, char *format, ...);
void printBytesFrom32Bit(int blockIndex, uint32_t v);
void printBlock(unsigned char *block, int nOfBlock, int size);
void printWordBlock(uint32_t *block, int nOfBlock);
void printExpandedWordBlock(uint32_t *block, int nOfBlock);
void printLogo();
char checkChar(char c);
void printRound(uint32_t T1, uint32_t T2, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g, uint32_t h, int round);
void printH(uint32_t H[8]);

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
#define logExpandendWordBlock(B, n) printExpandedWordBlock(B, n)
#define logBlock(B, n, s) printBlock(B, n, s)
#define logRound(T1, T2, a, b, c, d, e, f, g, h, i) printRound(T1, T2, a, b, c, d, e, f, g, h, i)
#define logH(H) printH(H)
#else
#define logDebug(T, ...)
#define phase(T, ...)
#define logo()
#define logExpandendWordBlock(B, n)
#define logBlock(B, n, s)
#define logRound(T1, T2, a, b, c, d, e, f, g, h, i)
#define logH(H)
#endif

#endif