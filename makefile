# compiler
CC = gcc

# include paths
INCLUDES = -I include -I .

# sdl (you will expand this later)
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image

# collect all c files in src
SRC = $(wildcard src/core/*.c) $(wildcard src/widgets/*.c)
OBJ = $(SRC:.c=.o)

# the output static library
LIB = libforms.a

# default
all: $(LIB)

$(LIB): $(OBJ)
	@echo "  AR    $@"
	ar rcs $@ $^

# pattern rule
%.o: %.c
	@echo "  CC    $<"
	$(CC) -c $< $(INCLUDES) -Wall -Wextra -std=c11

clean:
	rm -f $(OBJ) $(LIB)

# example executable (your test main)
example:
	$(CC) examples/main.c $(INCLUDES) -L. -lforms $(LIBS) -o example
