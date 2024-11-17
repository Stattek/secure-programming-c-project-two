.PHONY: debug clean run setup build

build/finalProgram: build/finalProgram.o build/davidRules.o
	gcc -o finalProgram build/finalProgram.o build/davidRules.o
	mv finalProgram build/

build/finalProgram.o: finalProgram.c
	gcc -c finalProgram.c
	mv finalProgram.o build/

build/davidRules.o: davidRules.c
	gcc -c davidRules.c
	mv davidRules.o build/

build:
	$(MAKE) setup
	$(MAKE) build/finalProgram

setup:
	@ if ! test -d build/; then mkdir build; fi

debug:
	$(MAKE) setup
	gcc -g -o finalProgram *.c
	mv finalProgram build/

clean:
	rm -r build/

run:
	./build/finalProgram
