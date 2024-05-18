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

# Wefx Drawing

A simple graphics library made in the spirit of [gfx](http://www.nd.edu/~dthain/courses/cse20211/fall2013/gfx)

To start out, we import our _wefx.h_ header file. This file contains a few structs, and
enumerations we will be using throughout this implementation file.

*/
#include "malloc.h"
#include "wefx.h"

typedef unsigned int color;

static unsigned int *buffer;
/*

We also defined some global variables for foreground and background colour, as well as a
global width (_w_) and height (_h_) variable.

*/
static color fg_color = 0;
static color bg_color = 0;
static int w = 0;
static int h = 0;

/*

## Opening a Canvas - wefx_open

Here we emulate opening a window. This allocates memory for our screen buffer.

*/
int wefx_open(unsigned int width, unsigned int height, const char *title)
{
    w = width;
    h = height;
    buffer = calloc(w * h, sizeof(int));
    if (buffer == NULL)
    {
        return 1;
    }
    return 0;
}
/*

## Convert Integers to RGB - rgb_to_int

Since we have have a few functions that deal with colors, we create an internal function that
will help convert three RGB values into a single integer value. This value will go directly
into the screen buffer to represent a pixel color.

When we are given three values to represent RGB, we shift the colors up into a single integer.
Note we need to ensure the values are not greater than 255 since we are only supporting RGB value
from within that range.

In the end, the integer will look like the following (in hexidecimal):

| alpha | blue | green | red |
|-------|------|-------|-----|
| 0xFF  | FF  | FF    | FF  |

from within a single integer.

*/
static int rgb_to_int(unsigned int red, unsigned int green, unsigned int blue)
{
    red = MIN(red, 255);
    green = MIN(green, 255);
    blue = MIN(blue, 255);
    int color = (0x00 << 24) + (blue << 16) + (green << 8) + (red);
    return color;
}
/*

## Set the Foreground Color - wefx_color

This function simply sets our _fg\_color_ variable to the specified color.

Subsequent calls to draw will use this color until it is changed.

*/
void wefx_color(unsigned int red, unsigned int green, unsigned int blue)
{
    fg_color = rgb_to_int(red, green, blue);
}
/*

## Draw a Single Point - wefx_point

This function sets one pixel value to a color. It set one of the values in
our _buffer_ array to the current value stored _fg\_color_.

By setting the value at $x + y * w$ we are drawing a point at $(x,y)$ on the screen.

*/
void wefx_point(int x, int y)
{
    int offset = x+(y*w);
    if(offset > w*h || offset < 0) {
        return;
    }
    buffer[offset] = fg_color;
}
/*

## Set the Background Color - wefx_clear_color

This color will be used as the background color for the image. Call this to
set the background color, and then by calling _wefx\_clear_, you can fill
the entire screen buffer.

*/
void wefx_clear_color(unsigned int red, unsigned int green, unsigned int blue)
{
    bg_color = rgb_to_int(red, green, blue);
}
/*

## Clear the Screen - wefx_clear

This function will "clear the screen". What it actually does is fill the whole screen
buffer with the current background color (see wefx_clear_color).

This is often called at the top of the render loop to reset to a blank slate before
doing any drawing.

*/
void wefx_clear()
{
    for (int q = 0; q < w * h; q++)
        buffer[q] = bg_color;
}
/*

## Draw a Line - wefx_line

Here we define a simple function to draw a line. It will draw from (x1,y1) to (x2,y2)
using Bresenham's line algorithm and the currently set foreground color [@BresenhamLineAlgorithm_2022_].

*/
void wefx_line(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    for (;;)
    {
        wefx_point(x0, y0);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * error;
        if (e2 >= dy)
        {
            if (x0 == x1)
                break;
            error = error + dy;
            x0 = x0 + sx;
        }
        if (e2 <= dx)
        {
            if (y0 == y1)
                break;
            error = error + dx;
            y0 = y0 + sy;
        }
    }
}
/*

## Draw a Circle - wefx_circle

This function can be called to draw a circle. It also uses the
currently set foreground color. It uses the Midpoint Circle Algorithm [@MidpointCircleAlgorithm_2022_].

*/
void wefx_circle(int x0, int y0, int r0)
{
    int x = r0;
    int y = 0;
    int err = 0;
    while (x >= y)
    {
        wefx_point(x0 + x, y0 + y);

        wefx_point(x0 + y, y0 + x);
        wefx_point(x0 - y, y0 + x);
        wefx_point(x0 - x, y0 + y);
        wefx_point(x0 - x, y0 - y);
        wefx_point(x0 - y, y0 - x);
        wefx_point(x0 + y, y0 - x);
        wefx_point(x0 + x, y0 - y);

        y += 1;
        err += 2 * y + 1;
        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}
/*

## Draw the Buffer to Screen

When we call any of the wefx draw functions, we are actually changing
the pixels in a buffer. The pixels we are setting are not actually
drawn to the screen.

This method is called from Javascript and asks us to draw our buffer to
what it considers to be the screen.

---

*Note*: there might be a faster / better way to do this.

---

*/
void wefx_draw(unsigned int *screen)
{
    for (int q = 0; q < w * h; q++)
        screen[q] = buffer[q];
}
