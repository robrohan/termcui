#include <inttypes.h>
#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>

#include <signal.h>
#include <sys/time.h>

#include "r2_termui.h"
#include "wefx.h"

#define FPS 30
#define FRAME_TIME (1.0 / FPS)

unsigned int ticks = 0;

static unsigned int *screen;

void render()
{
    printf(ESC_HIDE_CURSOR);
    wefx_draw(screen);

    unsigned int pixel_color;
    for (int y = 0; y < 30; y++)
    {
        for (int x = 0; x < 100; x++)
        {
            printf(ESC_CURSOR_POS, y, x);
            pixel_color = screen[x + (y * 100)];
            if (pixel_color > 0)
            {
                pixel_color = 34; // blue
                printf(ESC_SET_ATTRIBUTE_MODE_1, pixel_color);
                printf("%c", '*');
            }
            else
            {
                printf(ESC_SET_ATTRIBUTE_MODE_1, 0);
                printf(" ");
            }
        }
    }
    printf(ESC_SHOW_CURSOR);
}

void update(double dt, unsigned int ticks)
{
    wefx_color(0xff, 0xff, 0xff);
    // wefx_point(10, 10);
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
        render();
        ticks++;
    }
}

static void resize_sig_handler(int sig)
{
    printf(ESC_ERASE_SCREEN);
    printf("resize!\n");
}

static void poll_sig_handler(int sig)
{
    // ?
}

void resize_handler(void)
{
    struct sigaction sigact;
    struct sigaction sigwinsize;
    struct itimerval itimer;

    sigact.sa_flags = SA_RESTART;
    sigact.sa_handler = poll_sig_handler;
    sigemptyset(&sigact.sa_mask);
    sigaction(SIGVTALRM, &sigact, NULL);

    //// Window resizing ////
    sigemptyset(&sigwinsize.sa_mask);
    sigwinsize.sa_flags = 0;
    sigwinsize.sa_handler = &resize_sig_handler;
    sigaction(SIGWINCH, &sigwinsize, NULL);

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 20 * 1000; /* 50 times per second? */
    itimer.it_value = itimer.it_interval;
    setitimer(ITIMER_VIRTUAL, &itimer, NULL);
}

void screen_setup()
{
    printf(ESC_ERASE_SCREEN);
    wefx_open(100, 30, "Hello World");
    screen = malloc(100 * 30 * sizeof(int));
}

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");

    screen_setup();
    resize_handler();
    game_loop();

    printf(ESC_SET_ATTRIBUTE_MODE_1, 0);
    return 0;
}
