#include <inttypes.h>
#include <locale.h>
// #include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
// #include <unistd.h>

#include "termcui.h"
#include "wefx.h"

#define FPS 30
#define FRAME_TIME (1.0 / FPS)

typedef unsigned int rune;

// clang-format off
#define do { L(c) (rune)c } while(0);
// clang-format on

unsigned int ticks = 0;

static unsigned int *screen;

rune c = L'*';

void render()
{
    // int h = 0;
    // int w = 0;
    // printf(ESC_CURSOR_POS, h, w);
    // printf("%lc", c);

    wefx_draw(screen);

    // printf("%d\n\r", screen[(31-30)*29]);

    unsigned int pixel_color;
    for (int y = 0; y < 30; y++)
    {
        for (int x = 0; x < 100; x++)
        {
            printf(ESC_CURSOR_POS, y, x);
            // printf("%lc", screen[x * 100 + y]);
            pixel_color = screen[x + (y * 100)];
            // printf("%u", pixel_color);
            if (pixel_color > 0)
            {
                // exit(1);
                pixel_color = 34; // blue
                printf(ESC_SET_ATTRIBUTE_MODE_1, pixel_color);
                printf("0");
            }
            else
            {
                printf(ESC_SET_ATTRIBUTE_MODE_1, 0);
                printf("X");
            }
            // }
        }
    }

    // if (w >= 30 && h >= 30) {
    // printf(ESC_ERASE_SCREEN);
    // }
    // // printf(ESC_CURSOR_POS, 0, 10);
    // printf("test");
}

// void show_fps(double dt) {
//   printf(ESC_CURSOR_POS, 0, 10);
//   printf("%d fps %2dx%2d", (int)dt, w, h);
// }

void update(double dt, unsigned int ticks)
{
    // h = (ticks % 40);
    // w = h;

    wefx_color(0xff, 0xff, 0xff);
    // wefx_point(0, 0); // out of bounds
    wefx_point(1, 1);
    wefx_point(3, 3);
    wefx_point(5, 5);
    wefx_point(50, 15);
    wefx_point(99, 29);
    // wefx_point(100, 30); // out of bounds
    wefx_line(1, 1, 99, 29);
    wefx_line(1, 29, 99, 1);
    wefx_circle(50, 15, 10);
}

void game_loop()
{
    clock_t last_time = clock();
    clock_t current_time;
    double delta_time;

    while (1)
    {
        current_time = clock();
        delta_time = (double)(current_time - last_time) / CLOCKS_PER_SEC;

        // Only proceed if enough time has passed
        if (delta_time >= FRAME_TIME)
        {
            // Update the last_time
            last_time = current_time;

            // Here you would place your game update logic
            // printf("Frame rendered. Delta time: %.5f seconds\n", delta_time);
            update(delta_time, ticks);

            // Calculate remaining time to sleep if the loop is too fast
            double sleep_time = FRAME_TIME - delta_time;
            if (sleep_time > 0)
            {
                struct timespec ts;
                ts.tv_sec = (time_t)sleep_time;
                ts.tv_nsec = (long)((sleep_time - ts.tv_sec) * 1e9);
                nanosleep(&ts, NULL);
            }
        }
        // printf(ESC_ERASE_SCREEN);
        render();
        ticks++;
    }
}

void screen_setup()
{
    printf(ESC_ERASE_SCREEN);
    // printf(ESC_SET_ATTRIBUTE_MODE_3, 0, 30, 47);

    wefx_open(100, 30, "Hello World");
    screen = malloc(100 * 30 * sizeof(int));

    // wefx_clear_color(0x00, 0x00, 0x00);
    // wefx_clear();
}

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    // printf(ESC_REPORT_DEVICE, 0);

    screen_setup();
    game_loop();

    printf(ESC_SET_ATTRIBUTE_MODE_1, 0);
    return 0;
}
