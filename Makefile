CC = gcc
CFLAGS = -Wall -Wextra -lm -g
SRC = tema2.c
EXE = quadtree

build:
		$(CC) -o $(EXE) $(SRC) $(CFLAGS)

run:
		./$(EXE)

.PHONY : clean
clean :
		rm -f $(EXE) *~