SRC_FILES=main.c fun_with_files.c

all: main

main: $(SRC_FILES)
	gcc -std=c99 -Wall -o $@ $(SRC_FILES)

clean:
	rm -rf main main.dSYM

run: main
	# Running our main function from file main.c
	./main
