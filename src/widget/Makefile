##
## Makefile for Adage widget set
##

## Variable definitions
CC=g++
CFLAGS=-Wall -ggdb -pg -O0 `sdl-config --cflags` -I..
LDFLAGS=`sdl-config --libs` -lSDL_ttf -lm
AR=ar
ARFLAGS=cr
OUT=../libwidget.a
OBJECTS=text.o button.o richtext.o widgetbase.o
PREVIEWOBJ=preview.o
CLEANFILES=

.PHONY: default preview clean

default: $(OUT)

preview: $(PREVIEWOBJ)

clean:
	rm -f $(OUT) $(OBJECTS) $(PREVIEWOBJ) $(CLEANFILES)

$(OUT): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $+

.cpp.o:
	$(CC) $(CFLAGS) -c $+
