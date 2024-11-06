CC = gcc
OBJ = main.o mods.o read_text.o
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC)  -o $(EXEC) $(OBJ)

main.o: main.c mods.h read_text.c read_text.h
	$(CC)  -c main.c

mods.o: mods.c mods.h 
	$(CC)  -c mods.c

read_text.o: read_text.c read_text.h
	$(CC)  -c read_text.c
clean:
	rm -f $(OBJ) $(EXEC)