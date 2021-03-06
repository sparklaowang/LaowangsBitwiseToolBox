#include "ppstr_parser.h"
#include "memprintf.h"
#include <ctype.h>

#define CUCH fmt[fmt_idx] // Current Char
uint16_t UINT8_2_UINT16(uint8_t low, uint8_t high) {
  return (static_cast<uint16_t>(high) << 8)  + low ;
}

uint16_t UINT8_2_UINT32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
  return ((static_cast<uint32_t>(b3) << 24 ) +
  (static_cast<uint32_t>(b2) << 16 ) +
  (static_cast<uint32_t>(b1) << 8 ) +
  b0);
}

uint8_t HIGH8_OF_UINT16(uint16_t u16) {
  return (static_cast<uint8_t>(u16 >> 8));
}

uint8_t LOW8_OF_UINT16(uint16_t u16) {
  return (static_cast<uint8_t>(u16 & 0x00FF));
}

uint8_t I_BYTE_OF_U64(uint64_t u64, size_t i) {
  return (static_cast<uint8_t>(u64 >> (i * 8)) & 0xFF);
}

uint16_t I_WORD_OF_U64(uint64_t u64, size_t i) {
  return (static_cast<uint16_t>(u64 >> (i*16) & 0x0FFFF));
}

uint8_t I_BYTE_OF_U32(uint32_t u32, size_t i) {
  return (static_cast<uint8_t>(u32 >> (i * 8)) & 0xFF);
}

uint8_t I_BYTE_OF_U16(uint16_t u16, size_t i) {
  return (static_cast<uint8_t>(u16 >> (i * 8)) & 0xFF);
}

void UINT16_2_UINT8ARRAY(uint16_t u16, uint8_t *u8_arr) {
  u8_arr[0] = I_BYTE_OF_U16(u16, 0);
  u8_arr[1] = I_BYTE_OF_U16(u16, 1);
}

uint16_t UINT8ARRAY_2_UINT16(uint8_t *u8_arr) {
  return UINT8_2_UINT16(u8_arr[0], u8_arr[1]);
}

int CONV_NEXT_I_BYTE(uint8_t *buf, size_t len) {
  int sum = 0;
  for(int i = 0; i < len; len ++){
     sum += buf[i] << (i * 8);
  }
  return sum;
}

void UINT32_2_UINT8ARRAY(uint32_t u32, uint8_t *u8_arr) {
  u8_arr[0] = I_BYTE_OF_U16(u32, 0);
  u8_arr[1] = I_BYTE_OF_U16(u32, 1);
  u8_arr[2] = I_BYTE_OF_U16(u32, 2);
  u8_arr[3] = I_BYTE_OF_U16(u32, 3);
}
typedef enum enum_state_memprintf{
  NORMAL,
  ESCAPE,
  BITWIDTH,
  POSTPROCESS
} STATE;

void memprintf(char *dest, size_t dest_len, const char *fmt, uint8_t *mem, size_t mem_len)
{
    STATE sta = NORMAL;
    int fmt_idx = 0;
    int dest_idx = 0;
    int start_bit = 0;
    char *pdest = dest;
    int ERR = 0;
    
    int bitwidth; // Used to Record Each Bitwidth

    char PP_STR[1024];
    int PP_STR_IDX = 0;

    CONVERTER cvt_local;
    char type_local;
    
    while(CUCH != '\0' && !ERR){
      #ifdef DEBUG
         printf("%c, %d, %d \n", CUCH, sta, dest_idx);
      #endif
      switch(sta){
      case(NORMAL):
          if( CUCH == '%'){
              sta = ESCAPE;
          } else {
              dest[dest_idx] = CUCH;
              dest_idx ++;
          }
      break;
      case(ESCAPE):
          bitwidth = 0;
          if(CUCH <= '9' && CUCH >= '0') {
              bitwidth += CUCH - '0';
              sta = BITWIDTH;
          } else {
              ERR = 1;
              break;
          }
      break;
      case(BITWIDTH):
          if(CUCH <='9' && CUCH >= '0') {
              bitwidth = bitwidth * 10 + CUCH - '0';
          } else if(try_format_next_n_bit_as_type(mem, 
                                  mem_len,
                                  start_bit, 
                                  bitwidth, 
                                  CUCH) >=0) { // Format Success
              if(convert_next_bit_to_cvt(&cvt_local, mem, mem_len, start_bit, bitwidth)) { // Convert Failed;
                  ERR = 1;
                  break;
              }
              type_local = CUCH;
              start_bit += bitwidth;
              if(fmt[fmt_idx + 1] == '[') {
                  memset(PP_STR, 0, sizeof(PP_STR));
                  PP_STR_IDX = 0;
                  sta = POSTPROCESS;
              } else {
                  dest_idx += format_cvt_as_type(dest + dest_idx,
                                                 dest_len - dest_idx,
                                                 cvt_local,
                                                 type_local);
                  sta = NORMAL;
              }
          } else {
              ERR = 1;
              break;
          }
      break;
      case(POSTPROCESS):
          if(CUCH == '[') {
              //Start
          } else if(isdigit(CUCH) || CUCH == '.' || CUCH == '-'
                    || CUCH == 'a' || CUCH == 's'
                    || CUCH == 'm' || CUCH == 'd') {
              //Content
              PP_STR[PP_STR_IDX] = CUCH;
              PP_STR_IDX ++;
          } else if(CUCH == ']'){
              //End
              double res = 0;
              // Temporary: Alway Format CVT as float when Post Proces
              if(type_local == 'u') {
                  cvt_local.sg = static_cast<float>(cvt_local.u32);
              } else if(type_local == 'd') {
                  cvt_local.sg = static_cast<float>(cvt_local.i32);
              }
              printf("BEFORE: %s PP, cvt = %d %f\n", PP_STR, cvt_local.i32, cvt_local.sg);
              if(post_process_cvt_by_str(&cvt_local, PP_STR, 'f')){
                ERR = 1;
                break;
              }
              printf("AFTER: PP, cvt = %d %f\n", cvt_local.i32, cvt_local.sg);
              dest_idx += format_cvt_as_type(dest + dest_idx,
                                             dest_len - dest_idx,
                                             cvt_local, 
                                             'f');
              sta = NORMAL;
          }
      break;
      }
      fmt_idx ++;
    }
    if(dest_idx + 1 < dest_idx) {
      dest[dest_idx +1] = '\0';
    }
}

int format_cvt_as_type(char *dest, 
                              size_t dest_len, 
                              CONVERTER cvt,
                              char type) {
    char tmp_buf[1024];
    memset(tmp_buf, 0, sizeof(tmp_buf));
    switch(type){
    case('d'):
    case('D'):  
        //signed int
        // TODO: This Do have some problem
        sprintf(tmp_buf, "%g", static_cast<double>(cvt.i32));
    break;
    case('u'):
    case('U'):  
        //Unsigned int
        sprintf(tmp_buf, "%g", static_cast<double>(cvt.u32));
    break;
    case('f'):
    case('F'):  
        //Float
        sprintf(tmp_buf, "%g", static_cast<double>(cvt.sg));

    break;
    case('g'):
    case('G'):
        //Double
        sprintf(tmp_buf, "%g", static_cast<double>(cvt.sg));
    break;
    case('N'):
    case('n'):
        //Ignore
    break;
    default:
        return -5;
    break;
    }
    if(strlen(tmp_buf) > dest_len) {
        return -6;
    }
    strcpy(dest, tmp_buf);
    return strlen(tmp_buf);
}


int try_format_next_n_bit_as_type(uint8_t *mem, 
                                  size_t mem_len, 
                                  int bs, 
                                  int bw, 
                                  char type){
    char dummy_dest[1024];
    CONVERTER cvt;
    convert_next_bit_to_cvt(&cvt,
                            mem,
                            mem_len,
                            bs,
                            bw);
    return format_cvt_as_type(dummy_dest,
                                     1024,
                                     cvt,
                                     type);
}

int GET_NTH_BIT_OF_MEM(void *mem, size_t mem_len, int idx) {
    // Endian could be a problem, beware of it if you are not on linux
    int byte_idx = idx / 8;
    int bit_idx = idx % 8;
    
    if(byte_idx >= mem_len) {
        //Beyond boundary
        return -1;
    }

    uint8_t bit_mask = 0x01 << bit_idx;
    
    if( ((uint8_t *)mem)[byte_idx] & bit_mask) {
        return 1;
    } else {
        return 0;
    }
}

int SET_NTH_BIT_OF_MEM(void *mem, size_t mem_len, int idx, int value) {
    // Endian could be a problem, beware of it if you are not on linux
    int byte_idx = idx / 8;
    int bit_idx = idx % 8;
    
    if(byte_idx >= mem_len) {
        //Beyond boundary
        return -1;
    }

    uint8_t bit_mask = 0x01 << bit_idx;

    if(value == 0){
      // Set A Zero:
      ((uint8_t*)mem)[byte_idx] &= ~bit_mask;
    } else {
      // Set A One
      ((uint8_t*)mem)[byte_idx] |= bit_mask;
    }
    return 0;
}

int convert_next_bit_to_cvt(CONVERTER *cvt,
                            uint8_t *mem, 
                            size_t mem_len,
                            int bs,
                            int bw){
    memset(cvt, 0, sizeof(CONVERTER));
    int idx;
    for(idx = 0; idx < bw; idx ++) {
        int this_bit;
        this_bit = GET_NTH_BIT_OF_MEM(mem, mem_len, bs + idx);
        if(this_bit < 0) { //GET_WRONG
            return -3;
        }
        if(SET_NTH_BIT_OF_MEM((void *)(cvt), 
                              sizeof(CONVERTER), 
                              idx, 
                              this_bit) < 0) { //SET_WRONG
            return -4;
        }
    }
    return 0;
}
