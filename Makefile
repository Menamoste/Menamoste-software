DIR_BIN = bin
DIR_OBJ = obj
DIR_SRC = src
BIN = $(DIR_BIN)/main
SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(subst $(DIR_SRC),$(DIR_OBJ), $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -g -Wall -Wextra -std=c99 
LDLIBS = -lm -lSDL2 -lSDL2_ttf

all: dirs $(BIN)

.PHONY: run
run: all
	@echo --- Running ---
	./$(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)

# Make temporary directories
.PHONY: dirs
dirs:
	mkdir -p $(DIR_BIN) $(DIR_OBJ)

-include $(DEP)
