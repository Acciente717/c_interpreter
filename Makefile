all: cint

cint: src/intermediateCommand.cc
	g++ -std=c++14 -I src/inc -O2 -c -o src/intermediateCommand.o src/intermediateCommand.cc

clean:
	rm src/*.o