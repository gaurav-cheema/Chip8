main: clean
	clear
	@echo "Compiling and Running main file"
	g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lSDL2main -lSDL2 -lSDL2_image -o main.o
	./main.o

test: clean
	clear
	@echo "Compiling and Running Test"
	g++ -std=c++17 test.cpp -I"include" -L"lib" -Wall -lSDL2main -lSDL2 -lSDL2_image -o test.o
	./test.o

clean:
	@echo "Removing Test executable"
	rm -f ./main.o ./test.o
