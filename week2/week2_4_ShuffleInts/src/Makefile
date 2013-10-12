SRC_FILES=main.c shuffle.c

all: main

main: $(SRC_FILES)
	gcc -Wall -g -std=c99 -o $@ $(SRC_FILES)

clean:
	rm -rf main main.dSYM

run: main
	# Running our main function from file main.c
	./main
