DIR_BIN = bin
DIR_OBJ = obj
DIR_SRC = src
BIN = $(DIR_BIN)/main
SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(subst $(DIR_SRC),$(DIR_OBJ), $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99
LDLIBS = -lm -lSDL2main -lSDL2 -lSDL2_ttf

# Platform-specific settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CPPFLAGS = -MMD
	LDFLAGS =
else ifneq (,$(findstring MINGW,$(UNAME_S)))
	CPPFLAGS = -MMD -IC:\Users\S064015\scoop\apps\sdl2\current\include -IC:\Users\S064015\scoop\apps\sdl2\current\include\SDL2 -IC:\Users\S064015\scoop\apps\sdl2-ttf\current\include
	LDFLAGS = -LC:\Users\S064015\scoop\apps\sdl2\current\lib -LC:\Users\S064015\scoop\apps\sdl2-ttf\current\lib -Wl,--subsystem,windows
else
	CPPFLAGS = -MMD
	LDFLAGS =
endif

all: dirs $(BIN)

.PHONY: run
run: all
	@echo --- Running ---
	./$(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)

# Make temporary directories
.PHONY: dirs
dirs:
	mkdir -p $(DIR_BIN) $(DIR_OBJ)

-include $(DEP)
