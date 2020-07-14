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

uint16_t UINT8_2_UINT16(uint8_t low, uint8_t high);

uint16_t UINT8_2_UINT32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);

uint8_t HIGH8_OF_UINT16(uint16_t u16);

uint8_t LOW8_OF_UINT16(uint16_t u16);

uint8_t I_BYTE_OF_U64(uint64_t u64, size_t i);

uint16_t I_WORD_OF_U64(uint64_t u64, size_t i);

uint8_t I_BYTE_OF_U32(uint32_t u32, size_t i);

uint8_t I_BYTE_OF_U16(uint16_t u16, size_t i);

void UINT16_2_UINT8ARRAY(uint16_t u16, uint8_t *u8_arr);

uint16_t UINT8ARRAY_2_UINT16(uint8_t *u8_arr);

void UINT32_2_UINT8ARRAY(uint32_t u32, uint8_t *u8_arr);

void memprintf(char *dest, size_t dest_len, const char *fmt, uint8_t *mem, size_t mem_len);

int GET_NTH_BIT_OF_MEM(void *mem, size_t mem_len, int idx);
int SET_NTH_BIT_OF_MEM(void *mem, size_t mem_len, int idx, int value);

int CONV_NEXT_I_BYTE(uint8_t *buf, size_t);

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

