
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

#ifndef  __LIB_FFT256_H__
#define  __LIB_FFT256_H__

#include <types.h>

typedef struct {
	s64_t real;
	s64_t imag;
} complex_num_t;

/*************************************************************************************
 * Function Name: pl_lib_fft256
 *
 * Description:
 *   FFT-256 forward transformation.
 * 
 * NOTE:
 *  use example:
 *
 * step1:
 * for (i = 0; i < 256; i++) {
 *   xi[i].real = f(i);
 *   xi[i].imag = 0;
 * }
 * 
 * step2:
 * pl_lib_fft256(xi, xo);
 * 
 * step3:
 * for (i = 0; i < 256; i++) {
 *    amplitude[i] = sqrt(xo[i].imag * xo[i].imag + xo[i].real * xo[i].real);
 * }
 *
 * Parameters:
 *  @xi: input xi1.
 *  @xo: input xi2.
 *
 * Return:
 *  void.
 ************************************************************************************/
int pl_lib_fft256(complex_num_t xi[256], complex_num_t xo[256]);

/*************************************************************************************
 * Function Name: pl_ifft256
 *
 * Description:
 *   FFT-256 inverse transformation.
 *
 * NOTE:
 *  use example:
 *
 * step1:
 * pl_lib_ifft256(xi, xo);
 *
 * step2:
 * for (i = 0; i < 256; i++) {
 *    amplitude[i] = sqrt(xo[i].imag * xo[i].imag + xo[i].real * xo[i].real);
 * }
 * 
 * step3:
 * 	Happy PlainOS!
 *
 * Parameters:
 *  @xi: input xi1.
 *  @xo: input xi2.
 *
 * Return:
 *  void.
 ************************************************************************************/
int pl_lib_ifft256(complex_num_t xi[256], complex_num_t xo[256]);

#endif /* __LIB_FFT256_H__ */
