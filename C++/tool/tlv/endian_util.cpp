/*
 * =====================================================================================
 *
 *       Filename:  endian_util.cpp
 *
 *    Description:  functions for 网络字节顺序与主机字节顺序互相转换
 *
 * =====================================================================================
 */

#include "endian_util.h"

#include <stdint.h>
#include <arpa/inet.h>
#include <endian.h>

#ifdef __BYTE_ORDER
#define IsLittleEndian() (__BYTE_ORDER == __LITTLE_ENDIAN)
#else
static inline bool IsLittleEndian() {
  uint16_t val = 0x1;
  return ((uint8_t *)&val)[0] == 1;
}
#endif

int8_t N2H(int8_t val) {
  return val;
}

uint8_t N2H(uint8_t val) {
  return val;
}

int16_t N2H(int16_t val) {
  return ntohs(val);
}

uint16_t N2H(uint16_t val) {
  return ntohs(val);
}

int32_t N2H(int32_t val) {
  return ntohl(val);
}

uint32_t N2H(uint32_t val) {
  return ntohl(val);
}

int64_t N2H(int64_t val) {
  return N2H((uint64_t)val);
}

uint64_t N2H(uint64_t val) {
  if (IsLittleEndian()) {
    val = ((0xFF00000000000000 & val) >> 56) |
          ((0x00FF000000000000 & val) >> 40) |
          ((0x0000FF0000000000 & val) >> 24) |
          ((0x000000FF00000000 & val) >> 8)  |
          ((0x00000000FF000000 & val) << 8)  |
          ((0x0000000000FF0000 & val) << 24) |
          ((0x000000000000FF00 & val) << 40) |
          ((0x00000000000000FF & val) << 56);
  }

  return val;
}

int8_t H2N(int8_t val) {
  return val;
}

uint8_t H2N(uint8_t val) {
  return val;
}

int16_t H2N(int16_t val) {
  return htons(val);
}

uint16_t H2N(uint16_t val) {
  return htons(val);
}

int32_t H2N(int32_t val) {
  return htonl(val);
}

uint32_t H2N(uint32_t val) {
  return htonl(val);
}

int64_t H2N(int64_t val) {
  return H2N((uint64_t)val);
}

uint64_t H2N(uint64_t val) {
  if (IsLittleEndian()) {
    val = ((0xFF00000000000000 & val) >> 56) |
          ((0x00FF000000000000 & val) >> 40) |
          ((0x0000FF0000000000 & val) >> 24) |
          ((0x000000FF00000000 & val) >> 8)  |
          ((0x00000000FF000000 & val) << 8)  |
          ((0x0000000000FF0000 & val) << 24) |
          ((0x000000000000FF00 & val) << 40) |
          ((0x00000000000000FF & val) << 56);
  }

  return val;
}
