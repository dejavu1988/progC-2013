SRC_FILES=main.c writing_and_reading.c

all: main

main: $(SRC_FILES)
	gcc -Wall -o $@ $(SRC_FILES)

clean:
	rm -f main

run: main
	# Running our main function from file main.c
	./main
