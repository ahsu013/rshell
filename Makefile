WORKIT = -Wall -Werror -ansi -pedantic

all:
	mkdir -p bin
	g++ $(WORKIT) src/rshell0.cpp -o bin/rshell
	g++ $(WORKIT) src/ls.cpp -o bin/ls2
rshell:
	mkdir -p bin
	g++ $(WORKIT) src/rshell0.cpp -o bin/rshell
ls:
	mkdir -p bin
	g++ $(WORKIT) src/ls.cpp -o bin/ls


