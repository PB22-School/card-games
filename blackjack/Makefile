CC=g++
STD=c++17

build/main: build/main.o build/Hand.o build/Deck.o build/Card.o build/randomNumbers.o build/BlackJack.o
	$(CC) -o $@ $^ -std=$(STD) -lncursesw

build/test: build/test.o
	$(CC) -o $@ $^ -std=$(STD) -lncursesw

build/%.o: src/%.cpp
	@mkdir -p build
	@$(CC) -MM -MT $@ $< > build/$*.d
	$(CC) -c -o $@ $< -std=$(STD)

-include build/*.d