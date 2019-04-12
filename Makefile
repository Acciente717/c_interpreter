all: cint

exceptions:	src/inc/exceptions.hh

intermediateCommand: src/inc/intermediateCommand.hh src/intermediateCommand.cc
	g++ -std=c++14 -O2 -c -I src/inc -o src/intermediateCommand.o src/intermediateCommand.cc

representations: src/inc/representations.hh

typeManager: src/inc/typeManager.hh

utils: src/inc/utils.hh src/utils.cc
	g++ -std=c++14 -O2 -c -I src/inc -o src/utils.o src/utils.cc

yaccInfoStructure: src/inc/yaccInfoStructure.hh src/yaccInfoStructure.cc
	g++ -std=c++14 -O2 -c -I src/inc -o src/yaccInfoStructure.o src/yaccInfoStructure.cc

cint: exceptions intermediateCommand representations typeManager utils yaccInfoStructure src/lex_parser.l src/grammar.y
	flex -o src/lex_parser.cc src/lex_parser.l
	byacc -o src/grammar.cc src/grammar.y
	g++ -std=c++14 -O2 -c -I src/inc -o src/grammar.o src/grammar.cc
	g++ -std=c++14 -O2 -o cint src/grammar.o src/intermediateCommand.o src/utils.o src/yaccInfoStructure.o -ly -lfl

clean:
	rm src/*.o src/lex_parser.cc src/grammar.cc cint
