# SHA-256

A C implementation of SHA-256 algorithm. 

> **Not for production use**
> This is a study project. Feel free to copy, edit, or learn from it.
>
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

## Debugging

Compile with debug mode enabled:

```bash
make DEBUG=1
```

## Performance Test

Compile with this

```bash
make PERFORMANCE=1
```

and run

```bash
./performance
```

## Nist Vectors Test

Compile with this

```bash
make NIST=1
```

```bash
./nist
```

---

## Docs

Here you can see the original NIST [paper](docs/nist.fips.180-4.pdf)
Here my optimization journey [optimization](docs/Optimization.md)
