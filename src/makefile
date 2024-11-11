.PHONY: debug clean run setup

build/finalProgram: build/finalProgram.o build/davidRules.o
	@ if ! test -d build/; then mkdir build; fi
	gcc -o finalProgram build/finalProgram.o build/davidRules.o
	mv finalProgram build/

build/finalProgram.o: finalProgram.c
	gcc -c finalProgram.c
	mv finalProgram.o build/

build/davidRules.o: davidRules.c
	gcc -c davidRules.c
	mv davidRules.o build/

setup:
	mkdir build

debug:
	@ if ! test -d build/; then mkdir build; fi
	gcc -g -o finalProgram *.c
	mv finalProgram build/

clean:
	rm -r build/

run:
	./build/finalProgram
