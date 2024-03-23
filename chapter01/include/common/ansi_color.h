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
#ifndef __COMMON_ANSI_COLOR_H__
#define __COMMON_ANSI_COLOR_H__

/*************************************************************************************
 * NOTE:
 *    ANSI escape codes are a standard for in-band signaling to control the cursor
 * location, color, and other options on text terminals. The ANSI x3.64 standard
 * was released in 1979 and later updated by the 5th edition ECMA-48 standard in 1991,
 * ISO/IEC 6429 in 1992, and ISO-8613-3 in 1995.
 *    ANSI escape codes are started using \e[ and contain numeric codes separated by
 * semicolons. The escape code is terminated with an 'm'.
 *
 * This header file is reference to https://tintin.mudhalla.net/info/ansicolor/
 *
 ************************************************************************************/
#define ANSI_COLOR_RESET               "\033[0m"  /* Set color and display state to default */
#define ANSI_COLOR_BOLD                "\033[1m"  /* Set color intensity to bold */
#define ANSI_COLOR_FAINT               "\033[2m"  /* Set color intensity to faint */
#define ANSI_COLOR_ITALIC              "\033[3m"  /* Display text as italic */
#define ANSI_COLOR_UNDERLINE           "\033[4m"  /* Display text as underline */
#define ANSI_COLOR_SLOW_BLINK          "\033[5m"  /* Blink less than 150 times a minute */
#define ANSI_COLOR_FAST_BLINK          "\033[6m"  /* Blink 150 times a minute or more */
#define ANSI_COLOR_REVERSE             "\033[7m"  /* Reverse foreground and background colors */
#define ANSI_COLOR_ERASE               "\033[8m"  /* Display text as blank spaces */
#define ANSI_COLOR_STRIKE_THROUGH      "\033[9m"  /* Display text as strikethrough */
#define ANSI_COLOR_DOUBLE_UNDERLINE    "\033[21m" /* Display text as double underline */

#define ANSI_COLOR_BOLD_OFF            "\033[22m" /* Bold: off */
#define ANSI_COLOR_ITALIC_OFF          "\033[23m" /* Italic: off */
#define ANSI_COLOR_UNDERLINE_OFF       "\033[24m" /* Underline: off Neither single or double */
#define ANSI_COLOR_SLOW_BLINK_OFF      "\033[25m" /* Slow blink: off */
#define ANSI_COLOR_FAST_BLINK_OFF      "\033[26m" /* Slow blink: off */
#define ANSI_COLOR_REVERSE_OFF         "\033[27m" /* Reverse video: off */
#define ANSI_COLOR_ERASE_OFF           "\033[28m" /* Erase: off */
#define ANSI_COLOR_STRIKE_THROUGH_OFF  "\033[29m" /* Strikethrough: Off */

#define ANSI_COLOR_FORE_BLACK          "\033[30m" /* Foreground: black */
#define ANSI_COLOR_FORE_RED            "\033[31m" /* Foreground: red */
#define ANSI_COLOR_FORE_GREEN          "\033[32m" /* Foreground: green */
#define ANSI_COLOR_FORE_YELLOW         "\033[33m" /* yellow, Faint yellow often looks brown */
#define ANSI_COLOR_FORE_BLUE           "\033[34m" /* Foreground: blue */
#define ANSI_COLOR_FORE_MAGENTA        "\033[35m" /* Foreground: magenta */
#define ANSI_COLOR_FORE_CYAN           "\033[36m" /* Foreground: cyan */
#define ANSI_COLOR_FORE_WHITE          "\033[37m" /* Foreground: white */
#define ANSI_COLOR_FORE_RESET          "\033[39m" /* Foreground: reset foreground color to default */

#define ANSI_COLOR_BACK_BLACK          '\033[40m' /* Background: black */
#define ANSI_COLOR_BACK_RED            '\033[41m' /* Background: red */
#define ANSI_COLOR_BACK_GREEN          '\033[42m' /* Background: green */
#define ANSI_COLOR_BACK_YELLOW         '\033[43m' /* Background: yellow */
#define ANSI_COLOR_BACK_BLUE           '\033[44m' /* Background: blue */
#define ANSI_COLOR_BACK_MAGENTA        '\033[45m' /* Background: magenta */
#define ANSI_COLOR_BACK_CYAN           '\033[46m' /* Background: cyan */
#define ANSI_COLOR_BACK_WHITE          '\033[47m' /* Background: white */
#define ANSI_COLOR_BACK_RESET          '\033[49m' /* Background: rest backround color to default */

#endif /* __COMMON_ANSI_COLOR_H__ */
