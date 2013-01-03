OUTPUT=capinator.exe
CC=gcc
CFLAGS=-Wall

all: $(OUTPUT)



$(OUTPUT): TEST.o argvments.o
	$(CC) $(CFLAGS) *.o -o $(OUTPUT)

clean:
	del *.o
	del $(OUTPUT)

rebuild: clean all