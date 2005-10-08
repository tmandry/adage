##
## Makefile for Adage utils
##

## Variable definitions
CC=g++
CFLAGS=-Wall -ggdb -pg -O0 `sdl-config --cflags` -I..
LDFLAGS=`sdl-config --libs` -lSDL_ttf -lm
AR=ar
ARFLAGS=crs
OUT=../libutil.a
OBJECTS=smartptr.o
CLEANFILES=

.PHONY: default clean

default: $(OUT)

clean:
	rm -f $(OUT) $(OBJECTS) $(CLEANFILES)

$(OUT): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $+

.cpp.o:
	$(CC) $(CFLAGS) -c $+
