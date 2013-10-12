CHECK_CFLAGS=$(shell pkg-config --cflags check)
CHECK_LDFLAGS=$(shell pkg-config --libs check)
SRC_FILES=test_fun_with_files.c ../src/fun_with_files.c tmc-check.c

all: test

test: $(SRC_FILES)
	gcc -std=c99 $(CHECK_CFLAGS) -Wall -o $@ $(SRC_FILES) $(CHECK_LDFLAGS)

clean:
	rm -rf test tmc_available_points.txt tmc_test_results.xml valgrind.log *.txt test.dSYM

get-points:
	./test --print-available-points

run: test
	# Printing available points
	./test --print-available-points
	# Running test. There should be one success and one failure.
	./test
