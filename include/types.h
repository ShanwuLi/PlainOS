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

#ifndef __PLAINOS_TYPES_H__
#define __PLAINOS_TYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef NULL
#define NULL                            ((void *)0)
#endif

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
typedef unsigned int                     uint_t;
typedef unsigned long int                ul_t;
typedef unsigned long long               ull_t;

typedef signed char                      char_t;
typedef signed short int                 shrt_t;
typedef signed long int                  int_t;
typedef long int                         l_t;
typedef signed long long                 ll_t;

#ifndef true
#define true                             1
#endif

#ifndef false
#define false                            0
#endif

#ifndef UINTPTR_T_MAX
#define UINTPTR_T_MAX                    (~((uintptr_t)0))
#endif

#ifndef UINTPTR_T_BITS
#define UINTPTR_T_BITS                   (sizeof(uintptr_t) << 3)
#endif

#ifndef UINT16_MAX
#define UINT16_MAX                       ((u16_t)0xffff)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX                       ((u32_t)0xffffffff)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX                       ((u64_t)0xffffffffffffffff)
#endif


#endif /* __PLAINOS_TYPES_H__ */
