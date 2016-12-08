### cryptint


**`cryptint`** is a PostgreSQL extension providing SKIP32 encryption for 32-bit integers, and XTEA encryption for 64-bit integers.

### SKIP32
SKIP32 is a block cipher with 24 rounds based on Skipjack, that encrypts 32-bit values with a 80-bit key.  
See [Skipjack (wikipedia)](https://en.wikipedia.org/wiki/Skipjack_(cipher))  
Be warned that this encryption algorithm was developed in the 80's and is considered extremely weak by today's standard.

`skip32_encrypt(int value, bytea key) returns int`  
Encrypt an int (32-bit signed) with SKIP32. `key` must be 10 bytes long.

`skip32_decrypt(int value, bytea key) returns int`  
Decrypt an int (32-bit signed) with SKIP32. `key` must be 10 bytes long.


### XTEA

XTEA is a block cipher with 64 rounds, that encrypts 64-bit values with a 128-bit key.  
See [XTEA (wikipedia)](https://en.wikipedia.org/wiki/XTEA)

`xtea_encrypt(bigint value, bytea key) returns bigint`  
Encrypt a bigint (64-bit signed) with XTEA. `key` must be 16 bytes long.

`xtea_decrypt(bigint value, bytea key) returns bigint`  
Decrypt a bigint (64-bit signed) encrypted with XTEA. `key` must be 16 bytes long.


### Installation
The Makefile uses the [PGXS infrastructure](https://www.postgresql.org/docs/current/static/extend-pgxs.html) to find include and library files, and determine the install location.  
Build and install with:
```shell
$ make
$ (sudo) make install
```

### Usage
Once the extension is installed as a shared library, run in SQL:
```sql
CREATE EXTENSION cryptint;
```
this will create the extension in the current schema of the current database.
See [CREATE EXTENSION](https://www.postgresql.org/docs/current/static/sql-createextension.html) in PostgreSQL documentation for options.

### Demo in psql:
```sql
postgres=# CREATE EXTENSION cryptint;

postgres=# SELECT i, enc AS encrypted, skip32_decrypt(enc, '\x00010203040506070809'::bytea) AS decrypted
            FROM (SELECT i, skip32_encrypt(i, '\x00010203040506070809'::bytea) AS enc
                  FROM generate_series(-7,5) as i) AS list;
```
Result:

	 i  |  encrypted  | decrypted
	----+-------------+-----------
	 -7 |  1919056679 |        -7
	 -6 | -1412297942 |        -6
	 -5 |   151416393 |        -5
	 -4 |  2080108073 |        -4
	 -3 |  1863639393 |        -3
	 -2 | -1709192983 |        -2
	 -1 |   462394163 |        -1
	  0 |   175586429 |         0
	  1 | -1687378603 |         1
	  2 |  1293871674 |         2
	  3 |  -431061768 |         3
	  4 |  -563311787 |         4
	  5 | -1586536625 |         5



```sql
postgres=# SELECT i, enc AS encrypted, xtea_decrypt(enc, '\x000102030405060708090a0b0c0d0e0f'::bytea) AS decrypted
            FROM (SELECT i, xtea_encrypt(i, '\x000102030405060708090a0b0c0d0e0f'::bytea) AS enc
                  FROM generate_series(-7,5) as i) AS list;
```

Result:

	  i  |      encrypted       | decrypted 
	 ----+----------------------+-----------
	  -7 | -1293292978079273268 |        -7
	  -6 | -3293879689420368007 |        -6
	  -5 | -7669992214422926170 |        -5
	  -4 | -8172870873157163041 |        -4
	  -3 | -8476655326143017308 |        -3
	  -2 |  6852925861493684753 |        -2
	  -1 | -5097246286506330434 |        -1
	   0 | -3036054577413801443 |         0
	   1 |   103200416458222088 |         1
	   2 | -2807732263776726059 |         2
	   3 |  3315832599386841135 |         3
	   4 |  3207778724708741372 |         4
	   5 | -7709649685193830947 |         5
