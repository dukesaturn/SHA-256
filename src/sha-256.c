#include "sha-256.h"

static int isPrime(uint32_t p)
{
    if (p <= 1)
        return 0;

    for (int i = sqrt(p); i > 1; i--)
    {
        if (p % i == 0)
            return false;
    }

    return true;
}

static void generatePrimes(int size, uint32_t *primes)
{
    phase("Generating primes\n");

    int j = 0;

    for (int i = 2; j != size; i++)
    {
        if (isPrime(i))
        {
            logDebug("Generated prime n.%d is %d!\n", j, i);
            primes[j++] = i;
        }
    }
}

/**
 * Generate a constant.
 *
 * Take $p$ and $r = \sqrt(p)$ (it would be also cbrt, just an example).
 * Now $fracPart = modf(r)$ and $fracPart * 2^{32}$
 * Now $C = floor(fracPart * 2^{32})$
 *
 * @param p: Prime number
 * @param rootFuncName: The target function selected name for logging.
 * @param func: Target function pointer
 */
static uint32_t generateConstant(unsigned int p, char *rootFuncName, rootFuncPtr func)
{
    logDebug(BOLD("Generating costant from %d\n"), p);

    double sqrt_p = func(p);
    double integralPart = 0;
    double modf_p = modf(sqrt_p, &integralPart);
    long unsigned int res = (modf_p * pow(2, 32));

    logDebug("\t\t\t↪ 0x%lx ✔\n", res);
    return res;
}

static void generateConstants(const uint32_t *primes, uint32_t *dest, size_t size, char *rootFuncName, rootFuncPtr func)
{
    phase("Generating costants\n");

    for (int i = 0; i < size; i++)
    {
        int p = primes[i];
        dest[i] = generateConstant(p, rootFuncName, func);
    }
}

static void convertInBigEndian(uint32_t *block)
{
    for (int i = 0; i < 16; i++)
    {
        if (__BYTE_ORDER == __LITTLE_ENDIAN)
        {

            block[i] = __builtin_bswap32(block[i]); /** @todo: Fix portability  */
        }
    }
}

static void setSizeInPadding(unsigned char *block, int size, int paddedSize)
{
    uint64_t l = size * 8;

    block[paddedSize - 1] = l & 0xFF;
    block[paddedSize - 2] = (l & 0xFF00) >> BYTE_IN_BITS;
    block[paddedSize - 3] = (l & 0xFF0000) >> (BYTE_IN_BITS * 2);
    block[paddedSize - 4] = (l & 0xFF000000) >> (BYTE_IN_BITS * 3);
}

static uint32_t shr(size_t p, uint32_t x)
{
    return x >> p;
}

static uint32_t rotr(size_t p, uint32_t x)
{
    return (x >> p) | (x << (UINT32_BITS - p));
}

/**
 * $ \sigma_0(x) = \mathrm{ROTR}^7(x) \oplus \mathrm{ROTR}^{18}(x) \oplus \mathrm{SHR}^3(x) $
 */
static uint32_t sigma0(uint32_t x)
{
    return rotr(7, x) ^ rotr(18, x) ^ shr(3, x);
}

/**
 * $ \sigma_1(x) = \mathrm{ROTR}^{17}(x) \oplus \mathrm{ROTR}^{19}(x) \oplus \mathrm{SHR}^{10}(x) $
 */
static uint32_t sigma1(uint32_t x)
{
    return rotr(17, x) ^ rotr(19, x) ^ shr(10, x);
}

/**
 * $ \Sigma_0(x) = \mathrm{ROTR}^2(x) \oplus \mathrm{ROTR}^{13}(x) \oplus \mathrm{ROTR}^{22}(x) $
 */
static uint32_t Sigma0(uint32_t x)
{
    return rotr(2, x) ^ rotr(13, x) ^ rotr(22, x);
}

/**
 * $ \Sigma_1(x) = \mathrm{ROTR}^6(x) \oplus \mathrm{ROTR}^{11}(x) \oplus \mathrm{ROTR}^{25}(x) $
 */
static uint32_t Sigma1(uint32_t x)
{
    return rotr(6, x) ^ rotr(11, x) ^ rotr(25, x);
}

/**
 * $ \mathrm{Ch}(e,f,g) = (e \land f) \oplus (\neg e \land g) $
 */
static uint32_t choose(uint32_t e, uint32_t f, uint32_t g)
{
    return (e & f) ^ (~e & g);
}

/**
 * $ \mathrm{Maj}(a,b,c) = (a \land b) \oplus (a \land c) \oplus (b \land c) $
 */
static uint32_t maj(uint32_t a, uint32_t b, uint32_t c)
{
    return (a & b) ^ (a & c) ^ (b & c);
}

static void generateWords(unsigned char *buff, int counter, int size, uint32_t *words)
{
    memcpy(words, buff, BLOCK_SIZE_BYTES);
    convertInBigEndian(words);
    logBlock(buff, counter, size);
}

/**
 *
 * After the 16th:
 *
 * $W[i] = W[i-16] + σ0(W[i-15]) + W[i-7] + σ1(W[i-2])$
 */
static void generateExpandedWords(int counter, uint32_t *src, uint32_t *dest)
{
    memcpy(dest, src, BLOCK_SIZE_BYTES);

    for (int i = W_LEN; i < EXP_W_LEN; i++)
    {
        dest[i] = dest[i - 16] + sigma0(dest[i - 15]) + dest[i - 7] + sigma1(dest[i - 2]);
    }
    logExpandendWordBlock(dest, counter);
}

static void compressionRounds(uint32_t *H, uint32_t *K, uint32_t *expandedWords)
{
    uint32_t a = H[0];
    uint32_t b = H[1];
    uint32_t c = H[2];
    uint32_t d = H[3];
    uint32_t e = H[4];
    uint32_t f = H[5];
    uint32_t g = H[6];
    uint32_t h = H[7];

    for (int i = 0; i < ROUNDS; i++)
    {
        /**
         * $T1 = h + Σ1(e) + Ch(e,f,g) + K[i] + W[i]$
         * $T2 = Σ0(a) + Maj(a,b,c)$
         */

        uint32_t T1 = h + Sigma1(e) + choose(e, f, g) + K[i] + expandedWords[i];
        uint32_t T2 = Sigma0(a) + maj(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
        logRound(T1, T2, a, b, c, d, e, f, g, h, i);
    }

    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;

    logH(H);
}

/**
 * Returns 0 if an error occurs,
 * 1 if everything is ok.
 */
int sha256(const char *input, long int messageSize, char *result)
{
    uint32_t primes[PRIMES_LEN], H[H_LEN], K[K_LEN];

    /**
     * @note: I know that in production pre-generated tables are adopted.
     * But, for studying purposes and to understand how these constants
     * are generated, I preferred to generate them dynamically.
     */
    generatePrimes(PRIMES_LEN, primes);
    generateConstants(primes, H, H_LEN, "sqrt", sqrt);
    generateConstants(primes, K, K_LEN, "cbrt", cbrt);

    /** +9 = 1 byte (0x80 padding bit) + 8 bytes (64-bit message length) */
    long int nOfBlocks = ceil((float)(messageSize + 9) / (float)BLOCK_SIZE_BYTES);
    long int paddedSize = nOfBlocks * BLOCK_SIZE_BYTES;

    unsigned char *data = (unsigned char *)malloc(paddedSize);

    if (!data)
    {
        perror("Error with allocation");
        return 0;
    }

    memset(data, 0, paddedSize);
    memcpy(data, input, messageSize);

    data[messageSize] = 0x80;
    setSizeInPadding(data, messageSize, paddedSize);

    unsigned char buff[BLOCK_SIZE_BYTES] = {0};
    uint32_t words[W_LEN] = {0};
    uint32_t expandedWords[EXP_W_LEN] = {0};

    for (int i = 0; i < nOfBlocks; i++)
    {
        phase("Read block n. %d\n", i);

        memcpy(buff, data + ((i * BLOCK_SIZE_BYTES)), BLOCK_SIZE_BYTES);
        /** 16 Words of 32-bit */
        generateWords(buff, i, paddedSize, words);
        /** 64 words of 32-bit: first 16 copied, remaining 48 generated */
        generateExpandedWords(i, words, expandedWords);
        compressionRounds(H, K, expandedWords);
    }

    sprintf(result, "%08x%08x%08x%08x%08x%08x%08x%08x", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
    free(data);
    return 1;
}
