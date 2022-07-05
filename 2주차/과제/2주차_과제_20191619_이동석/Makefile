cc=gcc
CFLAGS = -W -g
target = main
objects = main.o io.o count.o

$(target) : $(objects)
	$(cc) $(CFLAGS) -o $(target) $(objects)

.c.o :
	$(cc) $(CFLAGS) -c $< -o $@

$(objects) : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)

