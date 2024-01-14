/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __KERNEL_TYPES_H__
#define __KERNEL_TYPES_H__

#include <stdint.h>

/*************************************************************************************
 * Type Name: type
 * Description: define a lots of  variable type.
 ************************************************************************************/
typedef uint8_t                          u8_t;
typedef uint16_t                         u16_t;
typedef uint32_t                         u32_t;
typedef uint64_t                         u64_t;

typedef int8_t                           s8_t;
typedef int16_t                          s16_t;
typedef int32_t                          s32_t;
typedef int64_t                          s64_t;

typedef unsigned char                    uchar_t;
typedef unsigned short int               ushrt_t;
typedef unsigned long int                uint_t;
typedef unsigned long long               ull_t;

typedef signed char                      char_t;
typedef signed short int                 shrt_t;
typedef signed long int                  int_t;
typedef signed long long                 ll_t;

#define UINTPTR_T_MAX                    (~((uintptr_t)0))
#define UINTPTR_T_BITS                   (sizeof(uintptr_t) << 3)

#endif /* __KERNEL_TYPES_H__ */