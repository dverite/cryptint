EXTENSION  = cryptint
EXTVERSION = 1.0.1

PG_CONFIG = pg_config

DATA = $(wildcard sql/*.sql)
MODULE_big = cryptint
OBJS      = cryptint.o xtea.o skip32.o

all:


PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
