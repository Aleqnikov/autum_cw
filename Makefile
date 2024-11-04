CC = gcc
OBJ = main.o mods.o 
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC)  -o $(EXEC) $(OBJ)

main.o: main.c mods.h 
	$(CC)  -c main.c

mods.o: mods.c mods.h 
	$(CC)  -c mods.c

clean:
	rm -f $(OBJ) $(EXEC)