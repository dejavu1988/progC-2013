#ifndef AES_H
#define AES_H

typedef uint8_t state [4][4];
typedef uint8_t Sbox [16][16];

typedef struct {
	uint8_t i;
	uint8_t j;
} intPair;

// Exercise 1
state* read_block(FILE *fp);

// Exercise 2
void shift_row(state *arr);
void inv_shift_row(state *arr);

// Exercise 3
intPair construct_indices(uint8_t byte);

// Exercise 4
Sbox* read_sbox(FILE *fp);

// Exercise 5
void sub_bytes(state *st, Sbox *box);

// Exercise 6
uint8_t mul1(uint8_t byte, int n);

// Exercise 7
uint8_t mul2(uint8_t byte1, uint8_t byte2);

// Execise 8
state* mix_columns(state *st);
// Exercise 9
state* inv_mix_columns(state *st);

// Exercise 10
state*  add_round_key(state *text, state *key);
#endif
