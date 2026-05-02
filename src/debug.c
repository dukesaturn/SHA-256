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

void printBytesFrom32Bit(int blockIndex, uint32_t v)
{
    uint8_t byte4 = (v & 0xFF000000) >> 24;
    uint8_t byte3 = (v & 0xFF0000) >> 16;
    uint8_t byte2 = (v & 0xFF00) >> 8;
    uint8_t byte1 = v & 0xFF;

    printf("  %04d  | 0x%8x          | %3d %3d %3d %3d | %c%c%c%c |\n",
           blockIndex,
           v,
           (byte4), (byte3), (byte2), (byte1),
           checkChar(byte4), checkChar(byte3), checkChar(byte2), checkChar(byte1));
}

void printWordBlock(uint32_t *block, int nOfBlock)
{
    printf("\033[1;97m Block    0x%017x                           \033[0m\n", nOfBlock);
    puts("────────────────────────────────────────────────────────");
    puts("\033[101m offset | hex                 | dec             | ascii \033[0m");
    puts("────────────────────────────────────────────────────────");
    for (int i = 0; i < 16; i++)
    {
        printBytesFrom32Bit(i + 1, block[i]);
    }
}

void printExpandedWordBlock(uint32_t *block, int nOfBlock)
{
    puts("────────────────────────────────────────────────────────");
    puts("EXPANDED BLOCK");

    puts("────────────────────────────────────────────────────────");
    puts("\033[101m offset | hex                 | dec             | ascii \033[0m");
    puts("────────────────────────────────────────────────────────");
    for (int i = 0; i < 64; i++)
    {
        printBytesFrom32Bit(i + 1, block[i]);
    }
}

void printBlock(unsigned char *block, int nOfBlock, int size)
{
    puts("────────────────────────────────────────────────────────");
    puts("SOURCE BLOCK");
    printf("─────────────────────────────────────Original─Size─%03dB─\n", size);
    printf("\033[44m\033[1;97m Block    0x%017x                           \033[0m\n", nOfBlock);
    puts("────────────────────────────────────────────────────────");
    puts(" offset | hex                 | dec             | ascii ");
    puts("────────────────────────────────────────────────────────");

    for (int i = 0; i < 64; i += 4)
    {
        printf("  %04d  | 0x%02x 0x%02x 0x%02x 0x%02x | %3u %3u %3u %3u | %c%c%c%c |\n",
               i,
               block[i], block[i + 1], block[i + 2], block[i + 3],
               block[i], block[i + 1], block[i + 2], block[i + 3],
               checkChar(block[i]), checkChar(block[i + 1]), checkChar(block[i + 2]), checkChar(block[i + 3]));
    }
}

void printRound(uint32_t T1, uint32_t T2, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g, uint32_t h, int round)
{
    if (round == 0)
    {
        puts("────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────");
        puts("ROUNDS");
        puts("────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────");
        puts("\033[46m\033[30m round |    T1      |    T2      |    a       |    b       |    c       |    d       |    e       |    f       |    g       |    h      \033[0m");
        puts("────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────");
    }

    printf("  %2d   | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x\n",
           round, T1, T2, a, b, c, d, e, f, g, h);
}

void printH(uint32_t H[8])
{
    puts("───────────────────────────────────────────────────────────────────────────────────────────────────────");
    puts("UPDATES H");
    puts("───────────────────────────────────────────────────────────────────────────────────────────────────────");
    puts("\033[46m\033[30m H[0]       | H[1]       | H[2]       | H[3]       | H[4]       | H[5]       | H[6]       | H[7]       \033[0m");
    puts("───────────────────────────────────────────────────────────────────────────────────────────────────────");
    printf(" 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x | 0x%08x\n",
           H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
    puts("───────────────────────────────────────────────────────────────────────────────────────────────────────");
}