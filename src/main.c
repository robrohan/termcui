#include <inttypes.h>
#include <locale.h>
// #include <math.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>

#include "wefx.h"
#include "termcui.h"

typedef unsigned int rune;

#define do { L(c) (rune)c } while(0);

unsigned int ticks = 0;

static unsigned int *screen;

rune c = L'*';

void render() {
  // int h = 0;
  // int w = 0;
  // printf(ESC_CURSOR_POS, h, w);
  // printf("%lc", c);

  wefx_draw(screen);

  for (int x = 100; x >= 0; x--) {
    for (int y = 30; y >= 0; y--) {
      // if (screen[x * 100 + y] != 0) {
        //  continue;
        printf(ESC_CURSOR_POS, y, x);
        // printf("%lc", screen[x * 100 + y]);
        printf("%lc", screen[x * 100 + y]);
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

void update(double dt, unsigned int ticks) {
  // h = (ticks % 40);
  // w = h;

  // wefx_circle(10, 10, 2);
  // wefx_line(0, 0, 100, 30);

  wefx_color(0xff, 0x00, 0x00);
  wefx_point(0, 0);
  wefx_point(100, 30);
 // wefx_point(0, 30);
 // wefx_point(100, 0);
}

void game_loop() {
  float fps = 1 / 5.0;
  float t = .0;

  float frame_time = .0;
  float delta_time = .0;

  // main loop
  for (;;) {
    // frame_time = (new_time - current_time) / 1e9;
    // current_time = new_time;

    while (frame_time > 0.0) {
      // delta_time = fmin(frame_time, fps);
      delta_time = frame_time < fps ? frame_time : fps;

      // physics(state, t, frame_time);
      update(delta_time, ticks);

      frame_time -= delta_time;
      t += frame_time;
    }

    // printf(ESC_ERASE_SCREEN);
    render();

    ticks++;
    // frames++;

    // if we hit 1 second, then set our timer to be the
    // current point and reset frame count
    // if (((int)time_in_seconds) > 0) {
    //   frames = 0;
    //   initial_time = sample_time;
    // }
    ////////////////////////////////////////////////
  }
}

void screen_setup() {
  printf(ESC_ERASE_SCREEN);
  // printf(ESC_SET_ATTRIBUTE_MODE_3, 0, 30, 47);

  wefx_open(100, 30, "Hello World");
  screen = malloc(100 * 30 * sizeof(int));
}

int main() {
  setlocale(LC_ALL, "en_US.UTF-8");
  // printf(ESC_REPORT_DEVICE, 0);
  
  screen_setup();
  game_loop();

  printf(ESC_SET_ATTRIBUTE_MODE_1, 0);
  return 0;
}
