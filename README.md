# SHA-256

![Algorithm: SHA-256](https://img.shields.io/badge/Algorithm-SHA--256-0a0a0a?style=for-the-badge&logo=security&logoColor=white)
![Platform: Linux](https://img.shields.io/badge/Platform-Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![License: GPL-2.0](https://img.shields.io/badge/License-GPL--2.0-blue?style=for-the-badge)

A C implementation of the SHA-256 hashing algorithm, developed by the NSA and standardized by NIST in 2001. Built as a learning exercise to study how hashing functions work internally.

> **Not for production use**
> This is a study project. Feel free to copy, edit, or learn from it.

> **Dependency note**
> This software uses GCC built-in functions and requires GCC to compile.

---

## Compiling

```bash
make
```

## Usage

```bash
./sha256 "Hello world!"
# c0535e4be2b79ffd93291305436bf889314e4a3faec05ecffcbb7df31ad9e51a
```

## Interactive debugging

Compile with debug mode enabled:

```bash
make DEBUG=1
```

The interactive shell shows each step of the algorithm:

![Block expansion](assets/1.png)

**Each block — original and expanded:**

![Block detail](assets/2.png)

**Compression rounds:**

![Compression rounds](assets/3.png)

---

## Testing

## Docs

Here you can see the original NIST [paper](docs/nist.fips.180-4.pdf)

## License

Distributed under the [GPL-2.0 License](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).