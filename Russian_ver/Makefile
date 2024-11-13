CC = gcc
OBJ = main.o mods.o text_tools.o
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC)  -o $(EXEC) $(OBJ)

main.o: main.c mods.h text_tools.c text_tools.h structures.h
	$(CC)  -c main.c

mods.o: mods.c mods.h 
	$(CC)  -c mods.c

text_tools.o: text_tools.c text_tools.h structures.h
	$(CC)  -c text_tools.c
clean:
	rm -f $(OBJ) $(EXEC)