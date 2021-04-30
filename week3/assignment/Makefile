CC = g++
OBJS = main.o string.o
TARGET = Str

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

$(OBJS) : string.h

.PHONY : clean
clean :
	rm $(OBJS) $(TARGET)
	