CC = clang
CFLAGS = -Wall -Wextra -Wpedantic
STATIC_LIBS = ./lib/libraylib.o
CLIBS = -lm

nor2doom: build/main.o $(STATIC_LIBS)
	$(CC) $(CFLAGS) $(CLIBS) $^ -o $@
	strip $@

build/main.o: src/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@
src/main.c:
	touch $@

clean: 
	rm -rf build
	rm -f ./nor2doom
