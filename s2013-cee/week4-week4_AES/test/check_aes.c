#include <check.h>
#include "tmc-check.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "../src/aes.h"

// Tests for construct_indices
START_TEST(test_construct_indices_with_empty_i_and_right_equals_one)
{
	intPair p = construct_indices(1);
	fail_unless(p.i == 0, "The left side of the parameters bits are empty, so i should be zero.");
	fail_unless(p.j == 1, "The rightmost bit is the only one set in the given input => j should be one");
}
END_TEST

START_TEST(test_construct_indices_with_empty_i_and_max_j)
{
	intPair p = construct_indices(15);
	fail_unless(p.i == 0, "The left bits are all zero, so i should be zero");
	fail_unless(p.j == 15, "The right bits are all set so j should be 15");
}
END_TEST

START_TEST(test_construct_indices_with_empty_i_and_j_1010)
{
	intPair p = construct_indices(10);
	fail_unless(p.i == 0, "The left bits are all zero, so i should be zero");
	fail_unless(p.j == 10, "Rightmost bits are set 1010, so result should be 10");
}
END_TEST

START_TEST(test_construct_indices_with_empty_j_and_min_i)
{
	intPair p = construct_indices(16);
	fail_unless(p.i == 1, "The first leftmost bit is one so i should be one");
	fail_unless(p.j == 0, "The rightmost bits are all zero so j should be zero");
}
END_TEST

START_TEST(test_construct_indices_with_empty_j_and_max_i)
{
	intPair p = construct_indices(240);
	fail_unless(p.i == 15, "The left bits are all set so i should be 15");
	fail_unless(p.j == 0, "The rightmost bits are all not set so j should be zero");
}
END_TEST

START_TEST(test_construct_indices_with_max_i_and_j_1010)
{
	intPair p = construct_indices(250);
	fail_unless(p.i == 15, "All leftmost bits are set so i should be 15");
	fail_unless(p.j == 10, "The rightmost bits are set 1010, so j should be 10");
}
END_TEST

// Tests for shift_row
START_TEST(test_shift_row_with_rows_0123)
{
	state *st = malloc(sizeof(state));
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			(*st)[i][j] = j;
		}
	}
	shift_row(st);

	// First row
	fail_unless((*st)[0][0] == 0, "The st[0][0] should be 0");
	fail_unless((*st)[0][1] == 1, "The st[0][1] should be 1");
	fail_unless((*st)[0][2] == 2, "The st[0][2] should be 2");
	fail_unless((*st)[0][3] == 3, "The st[0][3] should be 3");

	// Second row
	fail_unless((*st)[1][0] == 1, "The st[1][0] should be 1");
	fail_unless((*st)[1][1] == 2, "The st[1][2] should be 2");
	fail_unless((*st)[1][2] == 3, "The st[1][2] should be 3");
	fail_unless((*st)[1][3] == 0, "The st[1][2] should be 0");

	// Third row
	fail_unless((*st)[2][0] == 2, "The st[2][0] should be 2");
	fail_unless((*st)[2][1] == 3, "The st[2][2] should be 3");
	fail_unless((*st)[2][2] == 0, "The st[2][2] should be 0");
	fail_unless((*st)[2][3] == 1, "The st[2][2] should be 1");

	// Last row
	fail_unless((*st)[3][0] == 3, "The st[3][0] should be 3");
	fail_unless((*st)[3][1] == 0, "The st[3][2] should be 0");
	fail_unless((*st)[3][2] == 1, "The st[3][2] should be 1");
	fail_unless((*st)[3][3] == 2, "The st[3][2] should be 2");
}
END_TEST

START_TEST(test_inv_shift_row_with_rows_0123)
{
	state *st = malloc(sizeof(state));
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			(*st)[i][j] = j;
		}
	}
	inv_shift_row(st);

	// First row
	fail_unless((*st)[0][0] == 0, "The st[0][0] should be 0");
	fail_unless((*st)[0][1] == 1, "The st[0][1] should be 1");
	fail_unless((*st)[0][2] == 2, "The st[0][2] should be 2");
	fail_unless((*st)[0][3] == 3, "The st[0][3] should be 3");

	// Second row
	fail_unless((*st)[1][0] == 3, "The st[1][0] should be 3");
	fail_unless((*st)[1][1] == 0, "The st[1][2] should be 0");
	fail_unless((*st)[1][2] == 1, "The st[1][2] should be 1");
	fail_unless((*st)[1][3] == 2, "The st[1][2] should be 2");

	// Third row
	fail_unless((*st)[2][0] == 2, "The st[2][0] should be 2");
	fail_unless((*st)[2][1] == 3, "The st[2][2] should be 3");
	fail_unless((*st)[2][2] == 0, "The st[2][2] should be 0");
	fail_unless((*st)[2][3] == 1, "The st[2][2] should be 1");

	// Last row
	fail_unless((*st)[3][0] == 1, "The st[3][0] should be 3");
	fail_unless((*st)[3][1] == 2, "The st[3][2] should be 0");
	fail_unless((*st)[3][2] == 3, "The st[3][2] should be 1");
	fail_unless((*st)[3][3] == 0, "The st[3][2] should be 2");
}
END_TEST

// Tests for sub_bytes
START_TEST(test_sub_bytes_with_state_full_of_ones_and_sbox_with_9_in_first_and_8_in_second)
{
	state *st = malloc(sizeof(state));
	int i,j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 2; j++) {
			(*st)[i][j] = 1;
		}
		for (j = 2; j < 4; j++) {
			(*st)[i][j] = 2;
		}
	}

	Sbox *box = malloc(sizeof(Sbox));
	(*box)[0][1] = 9;
	(*box)[0][2] = 8;

	sub_bytes(st, box);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 2; j++) {
			fail_unless((*st)[i][j] == 9, "All values on the left side should be converted to nines.");
		}
		for (j = 2; j < 4; j++) {
			fail_unless((*st)[i][j] == 8, "All values on the right side should be converted to eights.");
		}
	}
}
END_TEST

START_TEST(test_sub_bytes_with_real_sbox)
{
    FILE *fp = fopen("test/sbox.txt", "r");
    Sbox *box = read_sbox(fp);
    fclose(fp);

    state *st = malloc(sizeof(state));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*st)[i][j] = j + i;
        }
    }

    sub_bytes(st, box);
    int vals[4][4] = {{99, 124, 119, 123}, {124, 119, 123, 242}, {119, 123, 242, 107}, {123, 242, 107, 111}};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*st)[i][j] == vals[i][j]);
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*st)[i][j] = j;
        }
    }

    sub_bytes(st, box);
    int exp[4][4] = {{99, 124, 119, 123}, {99, 124, 119, 123}, {99, 124, 119, 123}, {99, 124, 119, 123}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*st)[i][j] == exp[i][j]);
        }
    }
}
END_TEST

// Tests for read_block
START_TEST(test_read_block_with_exactly_128_bits)
{
	uint8_t vals[4][4] = {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}};
    FILE *fp = fopen("test/file_with_exactly_128_bits.txt", "r");
	state *st = read_block(fp);
	fclose(fp);

	int i,j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
            fail_unless((*st)[i][j] == vals[i][j], "Each row should contain values 0 1 2 3");
		}
	}
}
END_TEST

START_TEST(test_reading_sbox)
{
    FILE *fp = fopen("test/sbox.txt", "r");
    Sbox *box = read_sbox(fp);
    fail_unless((*box)[0][1] == 0x7c);
    fail_unless((*box)[5][3] == 0xed);
    fail_unless((*box)[15][14] == 0xbb);
}
END_TEST

// mul1 and mul2 tests
START_TEST (test_with_example_given_by_karvi)
{
	uint8_t byte = 103;
	uint8_t result = mul1(byte, 2);
	uint8_t expected = 135;
	fail_unless(result == expected, "The result was not what was expected in the example given in the exercise description.");
}
END_TEST

START_TEST (test_with_zero_byte_to_second_power)
{
	uint8_t result = mul1(0, 2);
	fail_unless(result == 0, "When multiplying 00000000 with x^2 something else except 00000000 was received as an answer.");
}
END_TEST

START_TEST (test_with_11111111_to_second_power)
{
	uint8_t result = mul1(255, 2);
	uint8_t expected = 209; // Computed previously on white board
	fail_unless(result == expected, "Multiplying 11111111 with x^2 did not yield the expected result.");
}
END_TEST

START_TEST (test_11111111_to_zero_power)
{
	uint8_t result = mul1(255, 0);
	fail_unless(result == 255);
}
END_TEST

// Tests for mul2
START_TEST (test_with_given_example_in_polynomial_form)
{
	uint8_t byte = 103;
	uint8_t poly = 4;
	uint8_t result = mul2(byte, poly);
	uint8_t expected = 135;
	fail_unless(result == expected, "The result was not what was gotten in the example given in the exercise description.");
}
END_TEST

START_TEST (test_with_11111111_to_second_power_equivalent_polynomial)
{
	uint8_t result = mul2(255, 4);
	fail_unless(result == 209);
}
END_TEST

START_TEST (test_with_11111111_to_zero_power_polynomial)
{
	uint8_t result = mul2(255, 1);
	fail_unless(result == 255);
}
END_TEST

START_TEST(test_with_10101010_and_10101010)
{
    uint8_t polynomial = 170;
    uint8_t result = mul2(polynomial, polynomial);
    fail_unless(result == 178, "10101010 mul2 10101010 should equal 31");
}
END_TEST

START_TEST(test_mix_columns_unit_state)
{
    uint8_t expected[4][4] = {{02, 03, 01, 01}, {01, 02, 03, 01}, {01, 01, 02, 03}, {03, 01, 01, 02}};
    state *st = malloc(sizeof(state));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*st)[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++) (*st)[i][i] = 1;

    state *new_st = mix_columns(st);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*new_st)[i][j] == expected[i][j], "When state is a unit matrix, mix columns should be exactly the constant matrix");
        }
    }
}
END_TEST

START_TEST(test_mix_columns_with_actual_example)
{
    uint8_t expected[4][4] = {{2, 3, 0, 9}, {7, 4, 13, 2}, {0, 9, 6, 7}, {5, 10, 11, 8}};
    state *st = malloc(sizeof(state));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*st)[i][j] = i + j;
        }
    }

    state *res = mix_columns(st);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*res)[i][j] == expected[i][j]);
        }
    }
}
END_TEST

START_TEST(test_inv_mix_columns_unit_state)
{
    uint8_t expected[4][4] = {{0x0E, 0x0B, 0x0D, 0x09}, {0x09, 0x0E, 0x0B, 0x0D}, {0x0D, 0x09, 0x0E, 0x0B}, {0x0B, 0x0D, 0x09, 0x0E}};
    state *st = malloc(sizeof(state));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*st)[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++) (*st)[i][i] = 1;

    state *new_st = inv_mix_columns(st);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*new_st)[i][j] == expected[i][j], "When state is a unit matrix, mix columns should be exactly the constant matrix");
        }
    }
}
END_TEST

START_TEST(test_inv_mix_columns_with_actual_state)
{
    uint8_t expected[4][4] = {{10, 43, 24, 49}, {15, 60, 21, 42}, {8, 33, 30, 63}, {13, 50, 19, 32}};
    state *st = malloc(sizeof(state));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*st)[i][j] = i + j;
        }
    }

    state *res = inv_mix_columns(st);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*res)[i][j] == expected[i][j]);
        }
    }
}
END_TEST

START_TEST(test_inv_mix_columns_reverses_mix_columns)
{
    state *st = malloc(sizeof(state));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*st)[i][j] = i + j;
        }
    }

    state *res = mix_columns(st);
    res = inv_mix_columns(res);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*res)[i][j] == i + j);
        }
    }
}
END_TEST

// Test add round key
START_TEST(test_add_round_key_with_example)
{
    state text = {{255, 255, 255, 255}, {240, 240, 240, 240}, {15, 15, 15, 15}, {1, 1, 1, 1}};
    state key = {{255, 255, 0, 0}, {255, 255, 15, 15}, {240, 240, 255, 255}, {0, 0, 1, 1}};

    state *result = add_round_key(&text, &key);

    fail_unless((*result)[0][1] == 0, "255 xor 255 should be 0");
    fail_unless((*result)[0][2] == 255, "255 xor 0 should be 255");
    fail_unless((*result)[1][0] == 15, "255 xor 240 should be 15");
    fail_unless((*result)[1][2] == 255, "240 xor 15 should be 255");
    fail_unless((*result)[2][1] == 255, "15 xor 240 should be 255");
    fail_unless((*result)[2][2] == 240, "255 xor 15 shouldbe 240");

    result = add_round_key(result, &key);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*result)[i][j] == text[i][j], "When adding round key again to the state, it should return to original state");
        }
    }
}
END_TEST

START_TEST(test_encrypt_decrypt)
{
    // Data initialization
    FILE *fp = fopen("test/encrypt_test_data.txt", "r");
    state *st = read_block(fp);
    fclose(fp);

    fp = fopen("test/encrypt_test_data.txt", "r");
    state *orig = read_block(fp);
    fclose(fp);

    fp = fopen("test/sbox.txt", "r");
    Sbox *box = read_sbox(fp);
    fclose(fp);

    fp = fopen("test/invbox.txt", "r");
    Sbox *inv_box = read_sbox(fp);
    fclose(fp);

    state *key = malloc(sizeof(state));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (*key)[i][i] = i + j;
        }
    }

    // Encrypt
    sub_bytes(st, box);
    shift_row(st);
    st = mix_columns(st);
    st = add_round_key(st, key);

    // Decrypt
    st = add_round_key(st, key);
    st = inv_mix_columns(st);
    inv_shift_row(st);
    sub_bytes(st, inv_box);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fail_unless((*st)[i][j] == (*orig)[i][j]);
        }
    }
}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test");

    // Tests for read_block
    tmc_register_test(s, test_read_block_with_exactly_128_bits, "w4-1.0");

    // Tests for shift_row
	tmc_register_test(s, test_shift_row_with_rows_0123, "w4-2.1");
	tmc_register_test(s, test_inv_shift_row_with_rows_0123, "w4-2.2");

    // Tests for construct_indicess
    tmc_register_test(s, test_construct_indices_with_empty_i_and_right_equals_one, "w4-3.1");
    tmc_register_test(s, test_construct_indices_with_empty_i_and_max_j, "w4-3.2");
    tmc_register_test(s, test_construct_indices_with_empty_i_and_j_1010, "w4-3.3");
    tmc_register_test(s, test_construct_indices_with_empty_j_and_min_i, "w4-3.4");
    tmc_register_test(s, test_construct_indices_with_empty_j_and_max_i, "w4-3.5");
    tmc_register_test(s, test_construct_indices_with_max_i_and_j_1010, "w4-3.6");

    tmc_register_test(s, test_reading_sbox, "w4-4.0");

    // Tests for sub_bytes
    tmc_register_test(s, test_sub_bytes_with_real_sbox, "w4-5.0");
    tmc_register_test(s, test_sub_bytes_with_state_full_of_ones_and_sbox_with_9_in_first_and_8_in_second, "w4-5.1");

    // Tests for mul1 and mul2
    tmc_register_test(s, test_with_example_given_by_karvi, "w4-6.0");
    tmc_register_test(s, test_with_zero_byte_to_second_power, "w4-6.0");
    tmc_register_test(s, test_with_11111111_to_second_power, "w4-6.0");
    tmc_register_test(s, test_11111111_to_zero_power, "w4-6.0");
    tmc_register_test(s, test_with_given_example_in_polynomial_form, "w4-7.0");
    tmc_register_test(s, test_with_11111111_to_second_power_equivalent_polynomial, "w4-7.0");
    tmc_register_test(s, test_with_11111111_to_zero_power_polynomial, "w4-7.0");
    tmc_register_test(s, test_with_10101010_and_10101010, "w4-7.0");

    // Test mix columns tests
    tmc_register_test(s, test_mix_columns_unit_state, "w4-8.0");
    tmc_register_test(s, test_mix_columns_with_actual_example, "w4-8.0");
    tmc_register_test(s, test_inv_mix_columns_unit_state, "w4-9.0");
    tmc_register_test(s, test_inv_mix_columns_with_actual_state, "w4-9.0");
    tmc_register_test(s, test_inv_mix_columns_reverses_mix_columns, "w4-9.0");

    // Test add round key
    tmc_register_test(s, test_add_round_key_with_example, "w4-10.0");

    tmc_register_test(s, test_encrypt_decrypt, "w4-11.0");

	return tmc_run_tests(argc, argv, s);
}
