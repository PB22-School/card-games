default:
	rm -rf build; mkdir build; g++ -std=c++11 src/main.cpp src/board.cpp src/ai.cpp -lncursesw -o build/main;
test:
	make default; ./build/main