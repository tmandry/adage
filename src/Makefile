#
# Makefile for Adage
# by Tyler Mandry
#

# Variable definitions
CC=g++
CFLAGS=-Wall -g -pg `sdl-config --cflags`
LDFLAGS=`sdl-config --libs` -lSDL_ttf -lm
OUT=adage
OBJECTS=main.o text.o button.o richtext.o

$(OUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

main.cpp: text.h
text.cpp: text.h
button.cpp: button.h
richtext.cpp: richtext.h
#input.cpp: text.h input.h

button.h: text.h
#input.h: text.h

%.o: %.cpp
	$(CC) $(CFLAGS) -c $+
