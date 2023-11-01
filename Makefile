EXTENSION  = cryptint
EXTVERSION = 1.0.1

PG_CONFIG = pg_config

DATA = sql/$(EXTENSION)--$(EXTVERSION).sql
MODULE_big = cryptint
OBJS      = cryptint.o xtea.o skip32.o

all:

#all: sql/$(EXTENSION)--$(EXTVERSION).sql 

#$(EXTENSION)--$(EXTVERSION).sql: sql/$(EXTENSION).sql
#	cp $< $@

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
