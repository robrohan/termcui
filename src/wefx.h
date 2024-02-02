/*
wefx

Copyright (c) 2022 Rob Rohan

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef WEFX__H
#define WEFX__H

#include "math.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

// Open a new graphics window.
int wefx_open(unsigned int width, unsigned int height, const char *title);

// Draw a point at (x,y)
void wefx_point(int x, int y);

void wefx_circle(int x0, int y0, int r0);

// Draw a line from (x1,y1) to (x2,y2)
void wefx_line(int x1, int y1, int x2, int y2);

// Change the current drawing color.
void wefx_color(unsigned int red, unsigned int green, unsigned int blue);

// Clear the graphics window to the background color.
void wefx_clear();

// Change the current background color.
void wefx_clear_color(unsigned int red, unsigned int green, unsigned int blue);

void wefx_draw(unsigned int *screen);

// Check to see if an event is waiting.
// int wefx_event_waiting();

#endif // WEFX__H