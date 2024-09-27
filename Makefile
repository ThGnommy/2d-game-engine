# Determine the operating system
UNAME_S := $(shell uname -s)

########################################
# Makefiles Variables
########################################

CC = g++
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c++17
INCLUDE_PATHS = -I"./libs/" -I"/opt/homebrew/include/" -L"/opt/homebrew/lib"

SRC_FILES = ./src/*.cpp ./src/**/*.cpp

LINKER_FLAGS = -lSDL2 \
				-lSDL2_image \
				-lSDL2_ttf \
				-lSDL2_mixer \
				-llua

OBJ_NAME = gameengine

########################################
# Makefiles Rules
########################################

ifeq ($(UNAME_S), Darwin) # Mac OS X
build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME);
else # Linux
build:
	g++ -Wall -std=c++17 -I"./libs/" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3 -o gameengine;
endif

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)