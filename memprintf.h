#ifndef MEMPRNTF_H
#define MEMPRNTF_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cvt.h"

#ifdef _cplusplus
extern "C"{
#endif

void memprintf(char *dest, size_t dest_len, const char *fmt, uint8_t *mem, size_t mem_len);

int GET_NTH_BIT_OF_MEM(void *mem, size_t mem_len, int idx);
int SET_NTH_BIT_OF_MEM(void *mem, size_t mem_len, int idx, int value);

int try_format_next_n_bit_as_type(uint8_t *mem, size_t mem_len, int bs, int bw, char type);
/*
int format_next_n_bit_as_type(char *dest, 
                              size_t length, 
                              uint8_t *mem, 
                              size_t mem_len, 
                              int bs, 
                              int bw, 
                              char type);
                              */
int format_cvt_as_type(char *dest, 
                       size_t length, 
                       CONVERTER cvt,
                       char type);
int convert_next_bit_to_cvt(CONVERTER *dest, 
                            uint8_t *mem,
                            size_t mem_len,
                            int bs,
                            int bw);

#ifdef _cplusplus
}
#endif


#endif

