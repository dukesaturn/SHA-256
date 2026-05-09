# Optimization

> The test is done on a Ubuntu VM. For having a comparison I've executed `openssl speed -evp sha256`
```bash
Doing sha256 for 3s on 16 size blocks: 8514023 sha256's in 2.97s
Doing sha256 for 3s on 64 size blocks: 5319395 sha256's in 2.99s
Doing sha256 for 3s on 256 size blocks: 3005882 sha256's in 2.99s
Doing sha256 for 3s on 1024 size blocks: 977551 sha256's in 2.99s
Doing sha256 for 3s on 8192 size blocks: 146867 sha256's in 2.99s
Doing sha256 for 3s on 16384 size blocks: 77481 sha256's in 3.00s

```

## Start point

First of all let's see with the previous primality test algorythm

```c

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
    int j = 0;

    for (int i = 2; j != size; i++)
    {
        if (isPrime(i))
        {
            primes[j++] = i;
        }
    }
}
```

Running the performance script

```bash
Doing sha256 ops for 3s on 16 size blocks: 332938 sha256 ops in 2.98s
Doing sha256 ops for 3s on 64 size blocks: 334608 sha256 ops in 2.98s
Doing sha256 ops for 3s on 1024 size blocks: 221017 sha256 ops in 2.98s
Doing sha256 ops for 3s on 8192 size blocks: 62219 sha256 ops in 2.98s
Doing sha256 ops for 3s on 16384 size blocks: 33510 sha256 ops in 2.98s

```

## Erathostene's algorithm

Simply, changing the primality test, we'll have

```bash
Doing sha256 ops for 3s on 16 size blocks: 490926 sha256 ops in 2.98s
Doing sha256 ops for 3s on 64 size blocks: 473836 sha256 ops in 2.98s
Doing sha256 ops for 3s on 1024 size blocks: 226083 sha256 ops in 2.98s
Doing sha256 ops for 3s on 8192 size blocks: 65627 sha256 ops in 2.98s
Doing sha256 ops for 3s on 16384 size blocks: 34731 sha256 ops in 2.98s
```

## Harcoding constants

Doing this the result is very better

```bash
Doing sha256 ops for 3s on 16 size blocks: 1442910 sha256 ops in 2.98s
Doing sha256 ops for 3s on 64 size blocks: 1278318 sha256 ops in 2.98s
Doing sha256 ops for 3s on 1024 size blocks: 424912 sha256 ops in 2.98s
Doing sha256 ops for 3s on 8192 size blocks: 71853 sha256 ops in 2.98s
Doing sha256 ops for 3s on 16384 size blocks: 36393 sha256 ops in 2.98s
```

## Deleting Buffer and various memcpy

- Before

```c
   for (int i = 0; i < nOfBlocks; i++)
    {
        memcpy(buff, data + ((i * BLOCK_SIZE_BYTES)), BLOCK_SIZE_BYTES);
        /** 16 Words of 32-bit */
        generateWords(buff, i, paddedSize, words);
        /** 64 words of 32-bit: first 16 copied, remaining 48 generated */
        generateExpandedWords(i, words, expandedWords);
        compressionRounds(H, K, expandedWords);
    }
```

After

```c

  for (size_t i = 0; i < nOfBlocks; i++)
    {
        /** 64 words of 32-bit: first 16 copied, remaining 48 generated */
        generateExpandedWords(expandedWords, data);
        compressionRounds(H, K, expandedWords);
    }
```

```bash
Doing sha256 ops for 3s on 16 size blocks: 1447716 sha256 ops in 2.98s
Doing sha256 ops for 3s on 64 size blocks: 1271587 sha256 ops in 2.98s
Doing sha256 ops for 3s on 1024 size blocks: 418484 sha256 ops in 2.98s
Doing sha256 ops for 3s on 8192 size blocks: 72165 sha256 ops in 2.98s
Doing sha256 ops for 3s on 16384 size blocks: 34208 sha256 ops in 2.98s
```

## Using static inline functions and compiling with -march=native

I've converted the function in `inline` 

```c
static inline uint32_t rotr(uint32_t p, uint32_t x)
{
    return ((x >> p) | (x << (UINT32_BITS - p)));
}
```

And this is the assembly

```asm
ror     esi, 5
```

Furthermore with this flag compilation

```bash
 gcc -O2 -march=native -o performance test/performance.c src/sha-256.c -lm
```

I've reached 

```bash
Doing sha256 ops for 3s on 16 size blocks: 1930468 sha256 ops in 2.98s
Doing sha256 ops for 3s on 64 size blocks: 1676303 sha256 ops in 2.98s
Doing sha256 ops for 3s on 256 size blocks: 1161771 sha256 ops in 2.98s
Doing sha256 ops for 3s on 1024 size blocks: 515066 sha256 ops in 2.98s
Doing sha256 ops for 3s on 8192 size blocks: 85940 sha256 ops in 2.98s
Doing sha256 ops for 3s on 16384 size blocks: 43942 sha256 ops in 2.98s
```