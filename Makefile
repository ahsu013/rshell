WORKIT = -Wall -Werror -ansi -pedantic

all:
	mkdir -p bin
	g++ $(WORKIT) src/rshell0.cpp -o bin/rshell
rshell:
	mkdir -p bin
	g++ $(WORKIT) src/rshell0.cpp -o bin/rshell



