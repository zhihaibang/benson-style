/*
 * =====================================================================================
 *
 *       Filename:  endian_util.h
 *
 *    Description:  functions for be and le byteorder converting
 *
 *        Version:  1.00
 *        Created:  2014/06/11
 *
 *         Author:  baronyuan
 *
 * =====================================================================================
 */

#ifndef __ENDIAN_UTIL_H__
#define __ENDIAN_UTIL_H__

#include <stdint.h>

int8_t N2H(int8_t val);
uint8_t N2H(uint8_t val);
int16_t N2H(int16_t val);
uint16_t N2H(uint16_t val);
int32_t N2H(int32_t val);
uint32_t N2H(uint32_t val);
int64_t N2H(int64_t val);
uint64_t N2H(uint64_t val);

int8_t H2N(int8_t val);
uint8_t H2N(uint8_t val);
int16_t H2N(int16_t val);
uint16_t H2N(uint16_t val);
int32_t H2N(int32_t val);
uint32_t H2N(uint32_t val);
int64_t H2N(int64_t val);
uint64_t H2N(uint64_t val);

#endif //!__ENDIAN_UTIL_H__
