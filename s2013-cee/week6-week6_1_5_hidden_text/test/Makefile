CHECK_CFLAGS=$(shell pkg-config --cflags check)
CHECK_LDFLAGS=$(shell pkg-config --libs check)
SRC_FILES=check_hidden_text.c tmc-check.c ../src/hidden_text.c

all: test

test: $(SRC_FILES)
	gcc -std=c99 -g $(CHECK_CFLAGS) -Wall -o $@ $(SRC_FILES) $(CHECK_LDFLAGS)

clean:
	rm -rf test tmc_available_points.txt tmc_test_results.xml valgrind.log test.dSYM mockoutput

run: test
	# Printing available points
	./test --print-available-points
	./test

get-points:
	./test --print-available-points
