CHECK_CFLAGS=$(shell pkg-config --cflags check)
CHECK_LDFLAGS=$(shell pkg-config --libs check)
SRC_FILES=check_function_ptrs.c tmc-check.c ../src/function_ptrs.c

all: test

test: $(SRC_FILES)
	gcc -std=c99 -g $(CHECK_CFLAGS) -Wall -o $@ $(SRC_FILES) $(CHECK_LDFLAGS)

clean:
	rm -rf test tmc_available_points.txt tmc_test_results.xml valgrind.log test.dSYM

run: test
	# Printing available points
	./test --print-available-points
	# Running test. There should be one success and one failure.
	./test

get-points:
	./test --print-available-points
