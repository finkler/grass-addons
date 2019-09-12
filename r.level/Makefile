GRASS_ROOT = /usr/lib/grass76
GRASS_INC = -I${GRASS_ROOT}/include
GRASS_LIB = -L${GRASS_ROOT}/lib -lgrass_gis -lgrass_raster

INCS = ${GRASS_INC}
LIBS = -lc -lm -lomp ${GRASS_LIB}

CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -O3 -mavx -D_DEFAULT_SOURCE -fopenmp ${INCS}
# CFLAGS = -g -std=c99 -pedantic -Wall -Wno-deprecated-declarations -O2 -D_DEFAULT_SOURCE ${INCS}
LDFLAGS = ${LIBS}
# LDFLAGS = -s ${LIBS}

CC = gcc
AR = ar

SRC = addr.c grid.c mask.c main.c
OBJ = ${SRC:.c=.o}

.c.o:
	@echo CC $<
	@${CC} -c -o $*.o ${CFLAGS} $<

all: r.level

r.level: ${OBJ}
	@echo LD $@
	@${CC} -o $@ $^ ${LDFLAGS}

clean:
	rm -f ${OBJ}

.PHONY: clean

