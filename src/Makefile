#
# Makefile for Adage
# by Tyler Mandry
#

# Variable definitions
CC=g++
CFLAGS=-Wall -ggdb -pg -O0 `sdl-config --cflags`
LDFLAGS=`sdl-config --libs` -lSDL_ttf -lm
OUT=adage
OBJECTS=main.o text.o button.o richtext.o widget.o manager.o

all: $(OUT)

clean:
	rm -rf $(OUT) $(OBJECTS) $(TEST)

$(OUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

main.cpp: text.h
text.cpp: text.h
button.cpp: button.h
richtext.cpp: richtext.h
handle_test.cpp: handle.h
#input.cpp: text.h input.h

button.h: text.h
#input.h: text.h

%.o: %.cpp
	$(CC) $(CFLAGS) -c $+
