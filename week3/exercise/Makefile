CC = g++
TARGET = result
OBJECTS = main.o array.o rangearray.o

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS) : Array.h RangeArray.h

.PHONY : clean
clean :
	rm $(TARGET) $(OBJECTS)