cc=g++ 
CPPFLAGS=-W -g -c

TARGET= main
OBJECTS= main.o array.o rangearray.o

$(TARGET) : $(OBJECTS) 
	$(cc) -o $(TARGET) $(OBJECTS)

$(OBJECTS) : Array.h RangeArray.h

.PHONY : clean
clean :
	rm $(OBJECTS) $(TARGET)
