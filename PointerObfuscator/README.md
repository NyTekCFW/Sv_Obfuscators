# PointerObfuscator
a C++ Runtime pointer obfuscation (C++11 +)

## Disclaimer
This obfuscator xor pointer address at runtime and unxor them at runtime.

It is compatible with all optimization levels (`-O0` to `-O3`, `-Os`).
Using `-s` (strip symbols) is strongly recommended to avoid exposing function names
and other string literals through debug or symbol information.
