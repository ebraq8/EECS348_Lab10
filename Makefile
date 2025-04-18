all: main

main: main.o calculator.o
	g++ -o main main.o calculator.o

main.o: main.cpp calculator.hpp
	g++ -c main.cpp

calculator.o: calculator.cpp calculator.hpp
	g++ -c calculator.cpp

clean:
	rm -f *.o main
