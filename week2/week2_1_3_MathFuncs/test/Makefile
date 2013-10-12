CHECK_CFLAGS=$(shell pkg-config --cflags check)
CHECK_LDFLAGS=$(shell pkg-config --libs check)
SRC_FILES=test_mathfuncs.c tmc-check.c ../src/mathfuncs.c

all: test

test: $(SRC_FILES)
	gcc $(CHECK_CFLAGS) -Wall -g -std=c99 -o $@ $(SRC_FILES) $(CHECK_LDFLAGS) -lm

clean:
	rm -rf test tmc_available_points.txt tmc_test_results.xml valgrind.log test.dSYM

run: test
	# Printing available points
	./test --print-available-points
	# Running test. There should be one success and one failure.
	./test

get-points:
	./test --print-available-points