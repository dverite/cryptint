CREATE OR REPLACE FUNCTION skip32_encrypt(int4,bytea)
RETURNS int4
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

COMMENT ON FUNCTION skip32_encrypt(int4,bytea)
IS 'encrypt an int4 with SKIP32 (10 bytes key)';



CREATE OR REPLACE FUNCTION skip32_decrypt(int4,bytea)
RETURNS int4
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

COMMENT ON FUNCTION skip32_decrypt(int4,bytea)
IS 'decrypt an int4 encrypted with SKIP32 (10 bytes key)';


CREATE OR REPLACE FUNCTION xtea_encrypt(bigint,bytea)
RETURNS bigint
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

COMMENT ON FUNCTION xtea_encrypt(bigint,bytea)
IS 'encrypt a bigint with XTEA (16 bytes key)';


CREATE OR REPLACE FUNCTION xtea_decrypt(bigint,bytea)
RETURNS bigint
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

COMMENT ON FUNCTION xtea_decrypt(bigint,bytea)
IS 'decrypt a bigint encrypted with XTEA (16 bytes key)';


