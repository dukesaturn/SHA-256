#include "debug.h"

void printLogo()
{
    puts("\033[38;5;43m  ███████╗██╗  ██╗ █████╗       ██████╗  ███████╗███████╗ ");
    puts("\033[38;5;79m  ██╔════╝██║  ██║██╔══██╗      ╚════██╗ ██╔════╝██╔════╝ ");
    puts("\033[38;5;116m  ███████╗███████║███████║█████╗ █████╔╝ ███████╗███████╗");
    puts("\033[38;5;153m  ╚════██║██╔══██║██╔══██║╚════╝██╔═══╝  ╚════██║██╔══██║ ");
    puts("\033[38;5;183m  ███████║██║  ██║██║  ██║      ███████╗ ███████║███████╗");
    puts("\033[38;5;189m  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝      ╚══════╝ ╚══════╝╚══════╝");
    puts("\033[0m  ─── sha-256 ↪ hashing ───");
}

void getFormattedTime(char *buf, size_t len)
{
    time_t timer = time(NULL);
    struct tm *currentTime = localtime(&timer);
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", currentTime);
}

void logMessage(char *level, char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char formattedTime[100];
    getFormattedTime(formattedTime, sizeof(formattedTime));
    printf("[%s] [%s] [%s:%d %s]  ", formattedTime, level, __FILE_NAME__, __LINE__, __func__);
    vprintf(format, ap);

    va_end(ap);
}

char checkChar(char c)
{
    if (!isprint(c))
        return '.';
    return c;
}

void printBytesFrom32Bit(int blockIndex, uint32_t src)
{
    uint32_t v = bswap_32(src); /** @todo: Fix portability  */
    uint8_t byte1 = (v & 0xFF000000) >> 24;
    uint8_t byte2 = (v & 0xFF0000) >> 16;
    uint8_t byte3 = (v & 0xFF00) >> 8;
    uint8_t byte4 = v & 0xFF;
    
    printf("  %04d  | 0x%2x 0x%2x 0x%2x 0x%2x | %3d %3d %3d %3d | %c%c%c%c |\n",
           blockIndex,
           checkChar(byte1), checkChar(byte2), checkChar(byte3), checkChar(byte4),
           checkChar(byte1), checkChar(byte2), checkChar(byte3), checkChar(byte4),
           checkChar(byte1), checkChar(byte2), checkChar(byte3), checkChar(byte4));
}

void printBlock(uint32_t *block, int nOfBlock)
{
    puts("────────────────────────────────────────────────────────");
    printf("\033[44m\033[1;97m Block    0x%017x                           \033[0m\n", nOfBlock);
    puts("────────────────────────────────────────────────────────");
    puts(" offset | hex                 | dec             | ascii ");
    puts("────────────────────────────────────────────────────────");
    for (int i = 0; i < 16; i++)
    {
        printBytesFrom32Bit(i + 1, block[i]);
    }
}