/*
 * cryptint.c
 * 
 * PostgreSQL functions to encrypt and decrypt int4 and int8 values with Skip32 and XTEA.
 */

#include "postgres.h"
#include "fmgr.h"

#if PG_VERSION_NUM >= 160000
#include "varatt.h"
#endif

#include "xtea.h"
#include "skip32.h"

PG_MODULE_MAGIC;

static int64 xtea_process(int64 clear, bytea* in_key_bytea, int encdec);
static int32 skip32_process(int32 clear, bytea* in_key_bytea, int encdec);

Datum xtea_encrypt(PG_FUNCTION_ARGS);
Datum xtea_decrypt(PG_FUNCTION_ARGS);

Datum skip32_encrypt(PG_FUNCTION_ARGS);
Datum skip32_decrypt(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(xtea_encrypt);
Datum
xtea_encrypt(PG_FUNCTION_ARGS)
{
	int64 r = xtea_process(PG_GETARG_INT64(0), PG_GETARG_BYTEA_PP(1), 1);
	PG_RETURN_INT64(r);
}

PG_FUNCTION_INFO_V1(xtea_decrypt);
Datum
xtea_decrypt(PG_FUNCTION_ARGS)
{
	int64 r = xtea_process(PG_GETARG_INT64(0), PG_GETARG_BYTEA_PP(1), 0);
	PG_RETURN_INT64(r);
}

/*
 * if encdec!=0, encrypt 64-bit value with 16-byte bytea key 
 * else decrypt value.
 * Return the encrypted or decrypted 64-bit result.
 */
static int64
xtea_process(int64 value, bytea* in_key_bytea, int encdec)
{
	unsigned char* in_key;
	unsigned int len = VARSIZE_ANY_EXHDR(in_key_bytea);
	uint32_t bufval[2];
	int64 res;
	uint32_t bufkey[4];
	int i;

	if (len != 16)
	{
		elog(ERROR, "XTEA encryption key must be 16 bytes long.");
		res = 0;
		goto exit;
	}

	/* Split the 64-bit value into two 32-bit blocks */
	bufval[0] = (uint32_t)(value >> 32);
	bufval[1] = (uint32_t)(value & 0xffffffff);

	/* Interpret in_key_bytea as if it contained 4 big-endian uint32.
	   The choice of big over little-endian is arbitrary */
	in_key = (unsigned char*)VARDATA_ANY(in_key_bytea);
	for (i=0; i < 4; i++)
	{
		bufkey[i] =	(uint32_t)in_key[0] << 24 |
				(uint32_t)in_key[1] << 16 |
				(uint32_t)in_key[2] << 8 |
				(uint32_t)in_key[3];
		in_key += 4;
	}

	if (encdec)
	  xtea_encipher(bufval, bufkey);
	else
	  xtea_decipher(bufval, bufkey);

	/* Join the two 32-bit blocks back into the 64-bit result */
	res = (((int64)bufval[0]) << 32) | bufval[1];
exit:
	return res;
}


PG_FUNCTION_INFO_V1(skip32_encrypt);
Datum
skip32_encrypt(PG_FUNCTION_ARGS)
{
	int32 r = skip32_process(PG_GETARG_INT32(0), PG_GETARG_BYTEA_PP(1), 1);
	PG_RETURN_INT32(r);
}

PG_FUNCTION_INFO_V1(skip32_decrypt);
Datum
skip32_decrypt(PG_FUNCTION_ARGS)
{
	int32 r = skip32_process(PG_GETARG_INT32(0), PG_GETARG_BYTEA_PP(1), 0);
	PG_RETURN_INT32(r);
}

/*
 * if encdec!=0, encrypt 32-bit value with 10-byte bytea key 
 * else decrypt value.
 * Return the encrypted or decrypted 32-bit result.
 */
static int32
skip32_process(int32 value, bytea* in_key_bytea, int encdec)
{
	unsigned char* in_key = (unsigned char*)VARDATA_ANY(in_key_bytea);
	unsigned int len = VARSIZE_ANY_EXHDR(in_key_bytea);
	unsigned char bufval[4];
	int32 res;

	if (len != 10)
	{
		elog(ERROR, "SKIP32 encryption key must be 10 bytes long.");
		res = 0;
		goto exit;
	}

	bufval[0] = (value & 0xff000000)>>24;
	bufval[1] = (value & 0xff0000)>>16;
	bufval[2] = (value & 0xff00)>>8;
	bufval[3] =  value & 0xff;

	fpe_skip32(in_key, bufval, encdec);

	res =   (bufval[0] << 24) |
		(bufval[1] << 16) |
		(bufval[2] << 8) |
		 bufval[3];
exit:
	return res;
}
