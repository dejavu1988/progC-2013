#ifndef HIDDENTEXT_H
#define HIDDENTEXT_H

uint8_t read_lsb(FILE* f);
uint8_t extract_byte(FILE* f);
uint32_t read_uint32(FILE* f);
uint32_t read_pixel_offset(FILE* f);
void read_msg(FILE* f);

#endif
