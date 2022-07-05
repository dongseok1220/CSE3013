OBJECTS = tetris.o
cc = gcc
CFLAGS = -g -W

TARGET = main

$(TARGET) : $(OBJECTS) tetris.h rank.h
	$(cc) -o $(TARGET) $(OBJECTS) -lncurses

clean : 
	rm $(OBJECTS) $(TARGET) rank.txt 
