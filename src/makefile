.PHONY: debug clean run

finalProgram: finalProgram.o davidRules.o
	@ if ! test -d build/; then mkdir build; fi
	gcc -o finalProgram finalProgram.o davidRules.o
	mv *.o build/
	mv finalProgram build/

finalProgram.o: finalProgram.c
	gcc -c finalProgram.c

davidRules.o: davidRules.c
	gcc -c davidRules.c

debug:
	@ if ! test -d build/; then mkdir build; fi
	gcc -g -o finalProgram finalProgram.c davidRules.c
	mv finalProgram build/

clean:
	rm -r build/

run:
	./build/finalProgram
