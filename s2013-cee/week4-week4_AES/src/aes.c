#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "aes.h"

const int STATE_ROWS = 4;
const int STATE_COLS = 4;
const int SBOX_ROWS = 16;
const int SBOX_COLS = 16;

state* read_block(FILE *fp)
{
    state* sp = (state*) malloc(sizeof(state));
    fread(sp, sizeof(uint8_t), STATE_ROWS*STATE_COLS, fp);
	return sp;
}

void shift_row(state *arr)
{
    uint8_t tmp;
    for(int i=1;i<STATE_ROWS;i++){
        int k = i;
        while(k--){
            tmp = (*arr)[i][0];
            (*arr)[i][0] = (*arr)[i][1];
            (*arr)[i][1] = (*arr)[i][2];
            (*arr)[i][2] = (*arr)[i][3];
            (*arr)[i][3] = tmp;
        }        
    }
}

void inv_shift_row(state *arr)
{
    uint8_t tmp;
    for(int i=1;i<STATE_ROWS;i++){
        int k = i;
        while(k--){            
            tmp = (*arr)[i][3];
            (*arr)[i][3] = (*arr)[i][2];
            (*arr)[i][2] = (*arr)[i][1];
            (*arr)[i][1] = (*arr)[i][0];
            (*arr)[i][0] = tmp;
        }        
    }
}

intPair construct_indices(uint8_t byte)
{
	intPair p;
    p.i = byte >> 4 & 0x0F;
    p.j = byte & 0x0F;
	return p;
}

Sbox* read_sbox(FILE *fp)
{
    Sbox* sp = (Sbox*) malloc(sizeof(Sbox));    
    unsigned int tmp;
    for(int i=0;i<SBOX_ROWS;i++){
        for(int j=0;j<SBOX_COLS;j++){
            fscanf(fp, "%2x", &tmp);
            (*sp)[i][j] = (uint8_t) tmp;
        }
    }
	return sp;
}

void sub_bytes(state *st, Sbox *box)
{
    intPair p;
    for(int i=0;i<STATE_ROWS;i++){
        for(int j=0;j<STATE_COLS;j++){
            p = construct_indices((*st)[i][j]);
            (*st)[i][j] = (*box)[p.i][p.j];
        }
    }
}

uint8_t mul1(uint8_t byte, int n) {
    int k = n; // n<8 && n>=0
    uint8_t tmp = byte;
    while(k--){
        if(tmp & 0x80){
            tmp = tmp << 1 ^ 0x1b;
        }else{
            tmp = tmp << 1;
        }
    }
    return tmp;
}

uint8_t mul2(uint8_t byte1, uint8_t byte2) {
    uint8_t tmp, n, res;
    tmp = byte1;
    n = 0;
    res = 0;
    while(tmp && n < 8){
        if(tmp & 0x01){
            res ^= mul1(byte2, n);
        }
        n++;
        tmp = tmp >> 1;
    }
    return res;
}


state* mix_columns(state *st) {
    const uint8_t coef[4][4] = {{2,3,1,1},{1,2,3,1},{1,1,2,3},{3,1,1,2}};
    state* s = (state*) malloc(sizeof(state));
    for(int i=0;i<STATE_ROWS;i++){
        for(int j=0;j<STATE_COLS;j++){
            (*s)[i][j] = mul2(coef[i][0],(*st)[0][j]) ^ mul2(coef[i][1],(*st)[1][j]) ^ mul2(coef[i][2],(*st)[2][j]) ^ mul2(coef[i][3],(*st)[3][j]);
        }        
    }
    return s;
}

state* inv_mix_columns(state *st) {
    const uint8_t coef[4][4] = {{0x0e,0x0b,0x0d,0x09},{0x09,0x0e,0x0b,0x0d},{0x0d,0x09,0x0e,0x0b},{0x0b,0x0d,0x09,0x0e}};
    state* s = (state*) malloc(sizeof(state));
    for(int i=0;i<STATE_ROWS;i++){
        for(int j=0;j<STATE_COLS;j++){
            (*s)[i][j] = mul2(coef[i][0],(*st)[0][j]) ^ mul2(coef[i][1],(*st)[1][j]) ^ mul2(coef[i][2],(*st)[2][j]) ^ mul2(coef[i][3],(*st)[3][j]);
        }        
    }
    return s;
}

state* add_round_key(state *text, state *key) {
    state* s = (state*) malloc(sizeof(state));
    for(int i=0;i<STATE_ROWS;i++){
        for(int j=0;j<STATE_COLS;j++){
            (*s)[i][j] = (*text)[i][j] ^ (*key)[i][j];
        }        
    }
    return s;
}


