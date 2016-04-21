// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how write text.
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

using namespace rgb_matrix;

static bool parseColor(Color *c, const char *str) {
  return sscanf(str, "%hhu,%hhu,%hhu", &c->r, &c->g, &c->b) == 3;
}

int main(int argc, char *argv[]) {
  Color color(255, 255, 0);
  const char *bdf_font_file = "fonts/5x7.bdf";
  int rows = 16;
  int chain = 1;
  int x_orig = 0;
  int y_orig = -1;

  int opt;

  if (bdf_font_file == NULL) {
    fprintf(stderr, "Need to specify BDF font-file with -f\n");
  }

  /*
   * Load font. This needs to be a filename with a bdf bitmap font.
   */
  rgb_matrix::Font font;
  if (!font.LoadFont(bdf_font_file)) {
    fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file);
  }

  /*
   * Set up GPIO pins. This fails when not running as root.
   */
  GPIO io;
  if (!io.Init())
    return 1;
    
  /*
   * Set up the RGBMatrix. It implements a 'Canvas' interface.
   */
  RGBMatrix *canvas = new RGBMatrix(&io, rows, chain);

  bool all_extreme_colors = true;
  all_extreme_colors &= color.r == 0 || color.r == 255;
  all_extreme_colors &= color.g == 0 || color.g == 255;
  all_extreme_colors &= color.b == 0 || color.b == 255;
  if (all_extreme_colors)
    canvas->SetPWMBits(1);

  const int x = x_orig;
  int y = y_orig;

  time_t now;
  struct tm *now_tm;
  int hour;
  int minute;

  now = time(NULL);
  now_tm = localtime(&now);
  hour = now_tm->tm_hour;
  minute = now_tm->tm_min;
  /*struct tm *tm_struct = localtime(time(NULL));
  int hour = localtime(time(NULL))->tm_hour;
  int minute = localtime(time(NULL))->tm_min;*/
  char curtime[5];
  sprintf(curtime, "%d:%d", hour, minute);

    rgb_matrix::DrawText(canvas, font, x, y + font.baseline(), color, curtime);
	sleep(10);

  // Finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
