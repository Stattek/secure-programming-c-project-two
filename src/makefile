.PHONY: debug clean run setup build

OBJECT_LIST=\
build/finalProgram.o \
build/davidRules.o \
build/xavierRules.o \
build/aayaanCWE.o

build/finalProgram: $(OBJECT_LIST)
	@ if ! test -d build/; then mkdir build; fi
	gcc -o finalProgram $(OBJECT_LIST)
	mv finalProgram build/

build/finalProgram.o: finalProgram.c
	gcc -c finalProgram.c
	mv finalProgram.o build/

build/davidRules.o: davidRules.c
	gcc -c davidRules.c
	mv davidRules.o build/

build/xavierRules.o: xavierRules.c
	gcc -c xavierRules.c
	mv xavierRules.o build/

build/aayaanCWE.o: aayaanCWE.c
	gcc -c aayaanCWE.c
	mv aayaanCWE.o build/

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
