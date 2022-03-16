#include <inttypes.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#include "tty.h"

typedef unsigned int rune;

#define do { L(c) (rune)c } while(0);

unsigned int ticks = 0;
int h = 0;
int w = 0;
rune c = L'🙂';

void render() {
  printf(ESC_CURSOR_POS, h, w);
  printf("%lc", c);

  if (w >= 19 && h >= 19) {
    printf(ESC_ERASE_SCREEN);
  }
}

void show_fps(double dt) {
  printf(ESC_CURSOR_POS, 0, 10);
  printf("%d fps %2dx%2d", (int)dt, w, h);
}

void update(double dt, unsigned int ticks) {
  h = (ticks % 20);
  w = h;
}

void game_loop() {
  float fps = 1 / 60.0;
  float t = .0;
  struct timespec current_time, new_time;
  clock_gettime(CLOCK_MONOTONIC_RAW, &current_time);
  float frame_time = .0;
  float delta_time = .0;

  ////////////////////////////////////////////////
  /// fps
  struct timespec initial_time, sample_time;
  unsigned int frames = 0;
  clock_gettime(CLOCK_MONOTONIC_RAW, &initial_time);
  ////////////////////////////////////////////////

  // main loop
  for (;;) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &new_time);
    frame_time = (new_time.tv_nsec - current_time.tv_nsec) / 1e9;
    current_time = new_time;

    while (frame_time > 0.0) {
      delta_time = fmin(frame_time, fps);

      // physics(state, t, frame_time);
      update(delta_time, ticks);

      frame_time -= delta_time;
      t += frame_time;
    }

    render();

    ticks++;
    frames++;

	////////////////////////////////////////////////
    /// fps
    clock_gettime(CLOCK_MONOTONIC_RAW, &sample_time);
    // get initial time ans sample time in nanoseconds
    float itime = initial_time.tv_sec * 1e9 + initial_time.tv_nsec;
    float stime = sample_time.tv_sec * 1e9 + sample_time.tv_nsec;
    // get the difference in time (in seconds)
    float time_in_seconds = (stime - itime) / 1e9;

    show_fps(frames / time_in_seconds);

    // if we hit 1 second, then set our timer to be the
    // current point and reset frame count
    if (((int)time_in_seconds) > 0) {
      frames = 0;
      initial_time = sample_time;
    }
   	////////////////////////////////////////////////
  }
}

void screen_setup() {
  printf(ESC_ERASE_SCREEN);
  // printf(ESC_SET_ATTRIBUTE_MODE_3, 0, 30, 47);
}

int main() {
  setlocale(LC_ALL, "en_US.UTF-8");
  // printf(ESC_REPORT_DEVICE, 0);
  
  screen_setup();
  game_loop();

  printf(ESC_SET_ATTRIBUTE_MODE_1, 0);
  return 0;
}
