#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "aes.h"

int main()
{
    FILE *fp = fopen("test/encrypt_test_data.txt", "r");
    state *st = read_block(fp);
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
    
    printf("Original state:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%x ", (*st)[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Encrypt
    sub_bytes(st, box);
    shift_row(st);
    st = mix_columns(st);
    st = add_round_key(st, key);
    
    printf("Encryption result:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%x ", (*st)[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Decrypt
    st = add_round_key(st, key);
    st = inv_mix_columns(st);
    inv_shift_row(st);
    sub_bytes(st, inv_box);
    
    printf("Decryption result:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%x ", (*st)[i][j]);
        }
        printf("\n");
    }
    printf("\n");
        
    return 0;
}
