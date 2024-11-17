.PHONY: debug clean run

setup:
	@ if ! test -d build/; then mkdir build; fi

build/finalProgram: build/finalProgram.o build/davidRules.o
	$(MAKE) setup
	gcc -o finalProgram build/finalProgram.o build/davidRules.o
	mv finalProgram build/

build/finalProgram.o: finalProgram.c
	gcc -c finalProgram.c
	mv finalProgram.o build/

build/davidRules.o: davidRules.c
	gcc -c davidRules.c
	mv davidRules.o build/

debug:
	$(MAKE) setup
	gcc -g -o finalProgram *.c
	mv finalProgram build/

clean:
	rm -r build/

run:
	./build/finalProgram
