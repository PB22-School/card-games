CC=g++
STD=c++17

default:
	make clean; make test

clean:
	rm -rf build

test:
	make build; ./build/main

build:
	mkdir build; $(CC) -std=$(STD) src/main.cpp src/randomNumbers.cpp src/Hand.cpp src/Deck.cpp src/Card.cpp src/BlackJack.cpp -lncursesw -o build/main;