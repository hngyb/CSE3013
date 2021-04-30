CC = gcc
TARGET = result
OBJECTS = main.o getPageNum.o countDigits.o

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS) : Header.h

.PHONY : clean
clean:
	rm $(TARGET) $(OBJECTS)
