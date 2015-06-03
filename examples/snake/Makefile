%.o: %.as
	../../build/arrow $< | opt -O3 -S | llc -filetype=obj -o $@

snake: main.o
	gcc -o $@ $^ -lSDL2

all: snake
