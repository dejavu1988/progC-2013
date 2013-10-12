#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "hidden_text.h"

/* Exercise 1 */
uint8_t read_lsb(FILE* fp) {
    uint8_t byte = 0;
    uint8_t* buf = (uint8_t*) malloc(sizeof(uint8_t));
    fread(buf, sizeof(uint8_t), 1, fp);
	byte = *buf & 1;
    free(buf);
    return byte;
}

/* Exercise 2 */
uint8_t extract_byte(FILE* fp) {
    uint8_t byte = 0;
    for(int i=0;i<8;i++){
        byte |= (read_lsb(fp) << i);
    }
    return byte;
}


/* Exercise 3 */
uint32_t read_uint32(FILE* f) {
    uint32_t uint32 = 0;
    uint32_t* buf = (uint32_t*) malloc(sizeof(uint32_t));
    fread(buf, sizeof(uint32_t), 1, f);
	uint32 = *buf;
    free(buf);
    return uint32;
}

/* Exercise 4 */
uint32_t read_pixel_offset(FILE* f) {
    fseek( f, 0xa, SEEK_SET );
    return read_uint32(f);
}
/* Exercise 5 */
void read_msg(FILE* f) {
    char c = NULL;
    uint32_t offset = read_pixel_offset(f);
    fseek( f, offset, SEEK_SET );
    while((c = extract_byte(f))){
        putchar(c);
    }
    putchar('\n');
}
