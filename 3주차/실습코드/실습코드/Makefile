cc=gcc
CFLAGS = -W -g
target = main
objects = main.o string_manipulation.o Output.o

$(target) : $(objects) 
	$(cc) $(CFLAGS) -o $(target) $(objects)


$(objects) : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
