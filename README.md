# gold_plated_zebra

Toy SHA1 implementation in x64 assembly using SHA extensions.


## Overview

This project is a learning exercise in assembly language. It should not be considered fit for use in any capacity.


## Build (Linux)

Assumes x64 and SHA1 extensions are available on the target X64 CPU.
<br />
Development tools:
```
$ sudo apt install gcc
$ sudo apt install nasm
$ sudo apt install build-essential
```
Tests require `CUnit`

```
$ sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
```
Finally
```
$ make
```

## Code example
```
#include "gpz.h"

uint8_t hash_ptr[20] = {}; // Digest
uint8_t msg_ptr[128] = {}; // Message
GPZ_SHA1 ctx;
GPZ_sha1_init(&ctx);
GPZ_sha1_update(&ctx, msg_ptr, 128);
GPZ_sha1_digest(&ctx, hash_ptr);

```

## Resources

Intel® SHA Extensions
<br />
https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sha-extensions.html

NIST FIPS 180-4 Secure Hash Standard (SHS)
<br />
https://csrc.nist.gov/pubs/fips/180-4/upd1/final

NIST Cryptographic Algorithm Validation Program
<br />
https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/secure-hashing

CUnit
<br />
https://cunit.sourceforge.net/
