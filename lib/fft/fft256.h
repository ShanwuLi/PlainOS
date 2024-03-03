
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

#ifndef  __PL_LIB_FFT256_H__
#define  __PL_LIB_FFT256_H__

#include <types.h>

typedef struct {
	s64_t real;
	s64_t imag;
} complex_num_t;

/*************************************************************************************
 * Function Name: pl_fft256
 *
 * Description:
 *   FFT-256 forward transformation.
 * 
 * Parameters:
 *  @xi: input xi1.
 *  @xo: input xi2.
 *
 * Return:
 *  void.
 ************************************************************************************/
int pl_fft256(complex_num_t *xi, complex_num_t *xo);

/*************************************************************************************
 * Function Name: pl_ifft256
 *
 * Description:
 *   FFT-256 inverse transformation.
 * 
 * Parameters:
 *  @xi: input xi1.
 *  @xo: input xi2.
 *
 * Return:
 *  void.
 ************************************************************************************/
int pl_ifft256(complex_num_t *xi, complex_num_t *xo);

#endif /* __PL_LIB_FFT256_H__ */
