cc = gcc
CFLAGS = -W -g
TARGET = animal.exe
objects = dog.o blackcow.o turtle.o main.o

$(TARGET) : $(objects)
	$(cc) -o $(TARGET) $(objects)

.c.o :
	$(cc) $(CFLAGS) -c $< -o $@


.PHONY : clean
clean:
	rm $(TARGET) $(objects)
