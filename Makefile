# Determine the operating system
UNAME_S := $(shell uname -s)

########################################
# Makefile Variables
########################################

CC = g++
COMPILER_FLAGS = -Wall -Wfatal-errors -g
LANG_STD = -std=c++17
SRC_FILES = ./src/*.cpp ./src/**/*.cpp
OBJ_NAME = gameengine

# Default linker flags (will be adjusted per OS)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua

# OS-specific settings
ifeq ($(UNAME_S), Darwin) # macOS
	INCLUDE_PATHS = -I"./libs/" -I"/opt/homebrew/include/" -L"/opt/homebrew/lib"
else # Linux
	INCLUDE_PATHS = -I"./libs/"
	# On Arch, SDL2 and Lua are usually installed globally with pkg-config
	# You can uncomment this to dynamically get the flags from pkg-config:
	# COMPILER_FLAGS += $(shell pkg-config --cflags sdl2)
	# LINKER_FLAGS += $(shell pkg-config --libs sdl2)
endif

########################################
# Makefile Rules
########################################

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm -f $(OBJ_NAME)
