default: build bin main

build bin:
	mkdir $@

clean:
	\rm -f build/*.o bin/main

main.o: main.cpp
	g++ -fopenmp -Wall -fomit-frame-pointer -O2 -c main.cpp -o build/main.o

main: main.o
	g++ -fopenmp -o bin/main build/main.o

debug.o: main.cpp
	g++ -fopenmp -Wall -fomit-frame-pointer -O2 -c main.cpp -o build/debug.o

debug: main.o
	g++ -fopenmp -o bin/main build/debug.o
