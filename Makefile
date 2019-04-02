all: cint

cint: src/intermediateCommand.cc
	g++ -O2 -c -o src/intermediateCommand.o intermediateCommand.cc

clean:
	rm src/*.o