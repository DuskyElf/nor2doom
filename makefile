CC = clang
CFLAGS = -ggdb -Wall -Wextra -Wpedantic
STATIC_LIBS = ./lib/libraylib.o
CLIBS = -lm

nor2doom: build/main.o build/logic_sim.o $(STATIC_LIBS)
	$(CC) $(CFLAGS) $(CLIBS) $^ -o $@

###  ---------------------- main --------------------------
build/main.o: src/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@
src/main.c: src/main.h src/logic_sim/logic_sim.h
	touch $@
src/main.h:
	touch $@

###  ---------------------- logic_sim --------------------------
build/logic_sim.o: build/logic_sim/logic_sim.o build/logic_sim/editor.o
	ld -r $^ -o $@

build/logic_sim/logic_sim.o: src/logic_sim/logic_sim.c
	mkdir -p build/logic_sim
	$(CC) $(CFLAGS) -c $^ -o $@
src/logic_sim/logic_sim.c: src/logic_sim/logic_sim.h src/logic_sim/editor.h
	touch $@
src/logic_sim/logic_sim.h: src/main.h
	touch $@

build/logic_sim/editor.o: src/logic_sim/editor.c
	$(CC) $(CFLAGS) -c $^ -o $@
src/logic_sim/editor.c: src/logic_sim/editor.h
	touch $@
src/logic_sim/editor.h: src/logic_sim/logic_sim.h src/main.h
	touch $@

clean: 
	rm -rf build
	rm -f ./nor2doom
