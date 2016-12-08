### cryptint


**`cryptint`** is a PostgreSQL extension providing encryption of 32-bit integers (int, int4) with Skip32 and 64-bit integers (bigint, int8) with XTEA.

### SKIP32
SKIP32 is a block cipher with 24 rounds based on Skipjack, that encrypts 32-bit values with a 80-bit key.
See https://en.wikipedia.org/wiki/Skipjack_(cipher)
Be warned that this encryption algorithm was developed in the 80's and is considered extremely weak by today's standard.

`skip32_encrypt(int value, bytea key) returns int`
Encrypt an int (32-bit signed) with SKIP32. `key` must be 10 bytes long.

`skip32_decrypt(int value, bytea key) returns int`
Decrypt an int (32-bit signed) with SKIP32. `key` must be 10 bytes long.


### XTEA

XTEA is a block cipher with 64 rounds, that encrypts 64-bit values with a 128-bit key.
See https://en.wikipedia.org/wiki/XTEA

`xtea_encrypt(bigint value, bytea key) returns bigint`
Encrypt a bigint (64-bit signed) with XTEA. `key` must be 16 bytes long.

`xtea_decrypt(bigint value, bytea key) returns bigint`
Decrypt a bigint (64-bit signed) encrypted with XTEA. `key` must be 16 bytes long.


