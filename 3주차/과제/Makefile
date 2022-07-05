cc=g++
CPPFALGS=-W -g -c

TARGET= main
OBJECTS = main.o str.o

$(TARGET) : $(OBJECTS)
	$(cc) -o $(TARGET) $(OBJECTS)

$(OBJECTS) : str.h

.PHONY : clean
clean :
	rm $(OBJECTS) $(TARGET)
