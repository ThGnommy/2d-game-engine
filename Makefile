# Determine the operating system
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin) # Mac OS X
build:
	g++ -Wall -std=c++17 -I"./libs/" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -o gameengine;
else # Linux
build:
	g++ -Wall -std=c++17 -I"./libs/" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3 -o gameengine;
endif

run:
	./gameengine

clean:
	rm gameengine