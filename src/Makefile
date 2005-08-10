##
## Makefile for Adage
##

## Variable definitions
CC=g++
CFLAGS=-Wall -ggdb -pg -O0 `sdl-config --cflags`
LDFLAGS=`sdl-config --libs` -lSDL_ttf -lm -L.
OUT=adage
OBJECTS=main.o
PREVIEWOUT=preview
PREVIEWOBJ=widget/preview.o
MODULES=widget utils
LIBRARIES=`echo '-l$(MODULES)' | sed 's/ / -l/g'`
CLEANFILES=gmon.out

.PHONY: all clean modules

all: $(OUT) $(PREVIEWOUT)

clean:
	rm -f $(OUT) $(OBJECTS) *.a $(CLEANFILES)
	@for i in $(MODULES) ; do \
	( cd $$i ; make clean ) ; \
	done

$(OUT): $(OBJECTS) modules
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBRARIES)

$(PREVIEWOUT): $(PREVIEWOBJ) modules
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(PREVIEWOBJ) $(LIBRARIES)

modules:
	@for i in $(MODULES) ; do \
	( cd $$i ; make ) ; \
	done

$(PREVIEWOBJ):
	@cd widget ; make preview

.cpp.o:
	$(CC) $(CFLAGS) -c $+	