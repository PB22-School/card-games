default:
	rm -rf build; mkdir build; g++ -std=c++11 src/main.cpp src/textWriter.cpp src/SoundPlayer.cpp src/InputReciever.cpp -lncursesw -o build/main
test:
	make default; ./build/main