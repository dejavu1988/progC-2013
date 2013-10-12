CHECK_CFLAGS=$(shell pkg-config --cflags check)
CHECK_LDFLAGS=$(shell pkg-config --libs check)
SRC_FILES=tmc-check.c test_skiplist.c ../src/decide_level.c ../src/skiplist.c

all: test

test: $(SRC_FILES)
	gcc -c -g  -std=c99 ../src/decide_level.c -o decide_level.o -Ddecide_level=__real_decide_level
	gcc -c -g -std=c99 ../src/skiplist.c -o skiplist.o
	gcc -g -std=c99 $(CHECK_CFLAGS) -Wall -o $@ tmc-check.c test_skiplist.c decide_level.o skiplist.o $(CHECK_LDFLAGS) 

clean:
	rm -rf decide_level.o skiplist.o test tmc_available_points.txt tmc_test_results.xml valgrind.log test.dSYM

get-points:
	./test --print-available-points

run: test
	# Printing available points
	./test --print-available-points
	# Running test. There should be one success and one failure.
	./test
