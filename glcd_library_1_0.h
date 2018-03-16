//***************************************************************************
//                     glcd_library.c
//
//  This file contains a library of routines for handling a graphics LCD
//
//
//****************************************************************************
//
//   GRAPHICS ROUTINES
//
//   glcd_line(x1,y1,x2,y2,colour)
//      * Draws a line from the first point to the second point
//        with the given colour.
//        - colour can be ON or OFF
//
//   glcd_rect(x1,y1,x2,y2,fill,colour)
//      * Draws a rectangle with upper left point (x1,y1) and lower
//        right point (x2,y2).
//        - fill can be YES or NO
//        - colour can be ON or OFF
//
//   glcd_bar(x1,y1,x2,y2,width,colour)
//      * Draws a bar (wide line) from the first point to the
//        second point.
//        - width is the number of pixels wide
//        - colour is ON or OFF
//
//   glcd_circle(x,y,radius,fill,colour)
//      * Draws a circle with center at (x,y)
//        - fill can be YES or NO
//        - colour can be ON or OFF
//
//******************************************************************************
//
//  TEXT WRITING ROUTINES
//
//  There are 5 fonts:
//
//   - 3x6 font and the associated code occupies approx 850 bytes
//     This is numeric and uppercase only and is very hard to read
//
//   - 5x7 font and the associated code occupies approx 1.2k bytes
//     This is a small but readable font giving 8 lines & 20 char per line
//
//   - 8x12 font and the associated code occupies approx 2K bytes
//     This is good for readability and gives 5 lines
//     at 16 characters per line (with size = 1).
// 
//   - 9x16 font and the associated code occupies approx 2.7K bytes
//     This is the best font if you have the space as it is very readable and
//     gives 4 lines at 14 characters per line (with size = 1).
//     
//   - 11x16 font and the associated code occupies approx 3.5K bytes
//     This is also very readable and gives 4 lines at 10 characters per line
//     (with size = 1).
//
//   Any or all together will be linked by calling the appropriate functions.
// 
//
//   Before calling ANY text function the starting position and optionally other
//   characteristics must be set by calling glcd_setxy()
//
//   void glcd_setxy(x, y, size, colour)
//       x and y: The upper left coordinate of the first character
//       size:    An integer that scales the size of the text (default is 1)
//       colour:  ON or OFF (default is ON)
//
//   If you are happy with the defaults you only need to specify x & y, for example:
//       glcd_setxy(0, 0);          // top left hand corner
//
//   All text functions update x and y automatically so that the invisible cursor is
//   ready for the next character.  The character \r will return the cursor to the
//   left margin and \n will cause the y position to step down a line.
//
//   Note that character wrapping is not supported, strings are truncated at the
//   edge of the display.
//
//   The data is stored in global variables and, as a shortcut, these can be accessed
//   directly.  The variables are:
//      int8 glcd_x, glcd_y         // Coordinates
//      int8 glcd_size              // Size
//      int glcd_colour            // Colour
//
//   The writing functions are:
//
//   glcd_putc36(char)
//      * Write char in 3x6 font
//
//   glcd_text36(textptr)
//      * Write the null terminated text pointed to by textptr in 3x6 font
//
//   glcd_putc57(char)
//      * Write char in 5x7 font
//
//   glcd_text57(textptr)
//      * Write the null terminated text pointed to by textptr in 5x7 font
//
//   glcd_putc812(char)
//      * Write char in 8x12 font
//      * Note that the tild character is replaced with a degrees C symbol
//
//   glcd_text812(textptr)
//      * Write the null terminated text pointed to by textptr in 8x12 font
//
//   glcd_putc916(char)
//      * Write char in 9x16 font
//      * Note that the tild character is replaced with a degrees C symbol
//
//   glcd_text916(textptr)
//      * Write the null terminated text pointed to by textptr in 9x16 font
//
//   glcd_putc1116(char)
//      * Write char in 11x16 font
//      * Note that the tild character is replaced with a degrees C symbol
//
//   glcd_text1116(textptr)
//      * Write the null terminated text pointed to by textptr in 11x16 font
//
//   The glcd_putcXX(char) functions can be used in printf().  For example:
//      printf(glcd_char812, "Temperature is %d~", temperature);
//      * Note that the tild (~) character is replaced with a degrees C symbol
//        in the 8x12 font
// 
//
//**********************************************************************************
//    LCD DRIVER
//
//   The above routines are dependent on a graphics driver that exposes the following
//   function:
//
//   glcd_pixel(x,y,colour)
//      * Sets the pixel to the given colour.
//        - colour can be ON or OFF
//
//
//   Other functions that the driver should supply (but are not used by this library) are:
//
//   glcd_init()
//      * Initilise the display
//      * Must be called before any other function.
//
//   glcd_fillScreen(colour)
//      * Fills the entire LCD with the given colour.
//        - colour can be ON or OFF
//
//
//**********************************************************************************



#ifndef __GLCD_LIB_H 
#define __GLCD_LIB_H


#include "ws0010.h"

typedef unsigned char bit;
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef short int int6;
//unsigned int6


extern byte glcd_x, glcd_y;                    // Coordinates used to specify where the next text will go
extern const byte glcd_size;                         // Same for size
//bit glcd_colour;                        // Same for colour

//#inline
//void glcd_setxy(x, y, sz = 1, cl = 1) {
//     glcd_x = x; glcd_y = y;            // set the coordinates
//     glcd_size = sz; glcd_colour = cl;  // set the other factors
//}

byte glcd_readByte(byte chip);
void glcd_writeByte(byte chip, byte data);
//void glcd_fillScreen(int colour);
void reset_caret();

int bit_test(int x, int y);

// Purpose:       Draw a line on a graphic LCD using Bresenham's
//                line drawing algorithm
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                colour - ON or OFF
// Dependencies:  glcd_pixel()
void glcd_line(int x1, int y1, int x2, int y2, int colour);


// Purpose:       Draw a rectangle on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                fill  - YES or NO
//                colour - ON or OFF
// Dependencies:  glcd_pixel(), glcd_line()
void glcd_rect(int x1, int y1, int x2, int y2, int fill, int colour);

// Purpose:       Draw a bar (wide line) on a graphic LCD
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                width  - The number of pixels wide
//                colour - ON or OFF
void glcd_bar(int x1, int y1, int x2, int y2, int width, int colour);


// Purpose:       Draw a circle on a graphic LCD
// Inputs:        (x,y) - the center of the circle
//                radius - the radius of the circle
//                fill - YES or NO
//                colour - ON or OFF
void glcd_circle(int x, int y, int radius, int fill, int colour);

// Purpose:       Write a char on a graphic LCD using the 3x6 bit font
void glcd_putc36(char c);

// Purpose:       Write text string in 3x6 font on a graphic LCD
void glcd_text36(char* textptr);

// Purpose:       Write a char on a graphic LCD using the 5x7 bit font
// Inputs:        (x,y) - The upper left coordinate of the first letter
//                textptr - A pointer to an array of text to display
//                size - The size of the text: 1 = 5x7, 2 = 10x14, ...
//                colour - ON or OFF
void glcd_putc57(char c);


// Purpose:       Write text string in 5x7 font on a graphic LCD
void glcd_text57(char* textptr);

// Purpose:       Write a char in 8x12 font on a graphic LCD
void glcd_putc812(char c);

// Purpose:       Write text string in 8x12 font on a graphic LCD
void glcd_text812(char* textptr);


// Purpose:       Write a char in 9x16 font on a graphic LCD
void glcd_putc916(char c);


// Purpose:       Write text string in 9x16 font on a graphic LCD
void glcd_text916(char* textptr);

// Purpose:       Write a char in 11x16 font on a graphic LCD
void glcd_putc1116(char c);

// Purpose:       Write text string in 11x16 font on a graphic LCD
void glcd_text1116(char* textptr);


#endif /*__ __GLCD_LIB_H */

