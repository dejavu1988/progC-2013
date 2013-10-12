MAIN_DIR = src
TEST_DIR = test

.PHONY: main test 

all: main test

main: 
	$(MAKE) -C $(MAIN_DIR)

test:
	$(MAKE) -C $(TEST_DIR)

clean:
	rm -rf tmc_available_points.txt tmc_test_results.xml valgrind.log tmc_memory_test_info.txt test.dSYM
	$(MAKE) -C $(MAIN_DIR) clean
	$(MAKE) -C $(TEST_DIR) clean

get-points:
	$(MAKE) -C $(TEST_DIR) get-points

run-main:
	$(MAKE) -C $(MAIN_DIR) run

run-test:
	$(MAKE) -C $(TEST_DIR) run
