.PHONY: debug clean run setup build

OBJECT_LIST=\
build/finalProgram.o \
build/davidCWE.o \
build/xavierCWE.o \
build/aayaanCWE.o \
build/waleedCWE.o \
build/coltonCWE.o

build/finalProgram: $(OBJECT_LIST)
	gcc -o finalProgram $(OBJECT_LIST)
	mv finalProgram build/

build/finalProgram.o: finalProgram.c
	gcc -c finalProgram.c
	mv finalProgram.o build/

build/davidCWE.o: davidCWE.c
	gcc -c davidCWE.c
	mv davidCWE.o build/

build/xavierCWE.o: xavierCWE.c
	gcc -c xavierCWE.c
	mv xavierCWE.o build/

build/aayaanCWE.o: aayaanCWE.c
	gcc -c aayaanCWE.c
	mv aayaanCWE.o build/

build/waleedCWE.o: waleedCWE.c
	gcc -c waleedCWE.c
	mv waleedCWE.o build/

build/coltonCWE.o: coltonCWE.c
	gcc -c coltonCWE.c
	mv coltonCWE.o build/

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
