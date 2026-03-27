## Build

```bash
make
```

## Run

```bash
make run
```

## macOS toolchain

On macOS, install dependencies with Homebrew:

```bash
brew install nasm qemu x86_64-elf-gcc
```

The `makefile` uses the `x86_64-elf-*` cross-toolchain by default on macOS.
