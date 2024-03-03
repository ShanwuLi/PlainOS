
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

#include "fft256.h"
#include <errno.h>

/*************************************************************************************
 * Description: reserve_series.
 *   Reverse mapping table.
 ************************************************************************************/
static const u8_t reserve_series[256] = {
	0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240, 8, 136, 72, 200,
	40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248, 4, 132, 68, 196, 36, 164, 100, 228,
	20, 148, 84, 212, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220,
	60, 188, 124, 252, 2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
	10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250, 6, 134, 70, 198,
	38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238,
	30, 158, 94, 222, 62, 190, 126, 254, 1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209,
	49, 177, 113, 241, 9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
	5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245, 13, 141, 77, 205,
	45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253, 3, 131, 67, 195, 35, 163, 99, 227,
	19, 147, 83, 211, 51, 179, 115, 243, 11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219,
	59, 187, 123, 251, 7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
	15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255
};

/*************************************************************************************
 * Description: The rotation factor of the reverse order mapping table is normalized
 *              here to adapt to integer operations
 * 
 *   WN[i]_Reality = WN[i]/100000.
 ************************************************************************************/
static const complex_num_t wn[128] = {
	{100000,0},{99969,2454},{99879,4906},{99729,7356},{99518,9801},
	{99247,12241},{98917,14673},{98527,17096},{98078,19508},{97570,21910},
	{97003,24298},{96377,26671},{95693,29028},{94952,31368},{94154,33689},
	{93299,35989},{92387,38268},{91421,40524},{90398,42755},{89322,44961},
	{88192,47139},{87008,49289},{85772,51410},{84485,53499},{83147,55557},
	{81758,57580},{80320,59569},{78834,61523},{77301,63439},{75720,65317},
	{74095,67155},{72424,68954},{70710,70710},{68954,72424},{67155,74095},
	{65317,75720},{63439,77301},{61523,78834},{59569,80320},{57580,81758},
	{55557,83147},{53499,84485},{51410,85772},{49289,87008},{47139,88192},
	{44961,89322},{42755,90398},{40524,91421},{38268,92387},{35989,93299},
	{33689,94154},{31368,94952},{29028,95693},{26671,96377},{24298,97003},
	{21910,97570},{19508,98078},{17096,98527},{14673,98917},{12241,99247},
	{9801,99518},{7356,99729},{4906,99879},{2454,99969},{0,100000},
	{-2454,99969},{-4906,99879},{-7356,99729},{-9801,99518},{-12241,99247},
	{-14673,98917},{-17096,98527},{-19508,98078},{-21910,97570},{-24298,97003},
	{-26671,96377},{-29028,95693},{-31368,94952},{-33689,94154},{-35989,93299},
	{-38268,92387},{-40524,91421},{-42755,90398},{-44961,89322},{-47139,88192},
	{-49289,87008},{-51410,85772},{-53499,84485},{-55557,83147},{-57580,81758},
	{-59569,80320},{-61523,78834},{-63439,77301},{-65317,75720},{-67155,74095},
	{-68954,72424},{-70710,70710},{-72424,68954},{-74095,67155},{-75720,65317},
	{-77301,63439},{-78834,61523},{-80320,59569},{-81758,57580},{-83147,55557},
	{-84485,53499},{-85772,51410},{-87008,49289},{-88192,47139},{-89322,44961},
	{-90398,42755},{-91421,40524},{-92387,38268},{-93299,35989},{-94154,33689},
	{-94952,31368},{-95693,29028},{-96377,26671},{-97003,24298},{-97570,21910},
	{-98078,19508},{-98527,17096},{-98917,14673},{-99247,12241},{-99518,9801},
	{-99729,7356},{-99879,4906},{-99969,2454}
};

/*************************************************************************************
 * Function Name: butterfly_calculate
 *
 * Description:
 *   Butterfly operation, normalization was performed during the calculation
 *   process to avoid data overflow and occurrence of clipping phenomenon.
 * 
 * Parameters:
 *  @xi1: input xi1.
 *  @xi2: input xi2.
 *  @xo1: output xo1.
 *  @xo2: output xo2.
 *  @wn_n: Butterfly coefficient.
 *
 * Return:
 *  void.
 ************************************************************************************/
static void butterfly_calculate(complex_num_t *xi1, complex_num_t *xi2,
                                complex_num_t *xo1, complex_num_t *xo2, int wn_n)
{
	complex_num_t times_pool = {0, 0};

	times_pool.real = xi2->real*wn[wn_n].real - xi2->real *wn[wn_n].imag;
	times_pool.imag = xi2->real*wn[wn_n].imag + xi2->imag *wn[wn_n].real;

	xo1->real = (xi1->real * 100000 + times_pool.real)/100000;
	xo1->imag = (xi1->imag * 100000 + times_pool.imag)/100000;
	xo2->real = (xi1->real * 100000 - times_pool.real)/100000;
	xo2->imag = (xi1->imag * 100000 - times_pool.imag)/100000;
}

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
int pl_fft256(complex_num_t *xi, complex_num_t *xo)
{
	int i;
	int j;
	int k;

	if (xi == NULL || xo == NULL)
		return -EFAULT;

	for(i = 0; i < 256; i++)
		*(xo + i) = *(xi + reserve_series[i]);

	for(i = 0; i<256; i++)
		*(xi + i) = *(xo + i);

	for(i = 1; i <= 128; i <<= 1) {
		for(j = 0; j < 128 / i; j++) {
			for(k = 0; k<i; k++)
				butterfly_calculate(xi + j * i * 2 + k, xi + j * i * 2 + k + i,
				                   (xo + j * i * 2 + k), (xo + j * i * 2 + k + i),
				                    k * (128 / i));
		}
	}

	for(k = 0; k < 256; k++)
		*(xi + k) = *(xo + k);

	return OK;
}

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
int pl_ifft256(complex_num_t *xi, complex_num_t *xo)
{
	int i;
	s64_t k;
	int ret;

	for(i = 0; i < 256; i++) {
		k = (xi + i)->imag;
		k = -k ;
		(xi + i)->imag = k;
	}

	ret = pl_fft256(xi, xo);
	return ret;
}

