
all: bin/main.exe

clean:
	rm -rf bin

bindir :
	mkdir -p bin


bin/display.o : ../src/display.cpp ../src/display.hpp bindir
	g++ ../src/display.cpp -c -o bin/display.o -Wall

bin/autom.o : ../src/autom.cpp ../src/autom.hpp bindir
	g++ ../src/autom.cpp -c -o bin/autom.o -Wall

bin/main.o : main.cpp ../src/display.hpp ../src/autom.hpp stub.hpp bindir
	g++ main.cpp -c -o bin/main.o -Wall

bin/main.exe : bin/main.o bin/display.o bin/autom.o
	g++ bin/main.o bin/display.o bin/autom.o -o bin/main.exe -Wall