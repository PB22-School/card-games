default:
	rm -rf build; mkdir build; g++ -std=c++11 src/main.cpp src/Poker.cpp src/randomNumbers.cpp src/Card.cpp src/Deck.cpp src/Hand.cpp -lncursesw -o build/main

test:
	make default; ./build/main