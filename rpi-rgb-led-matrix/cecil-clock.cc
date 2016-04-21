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

/*  if (isatty(STDIN_FILENO)) {
    // Only give a message if we are interactive. If connected via pipe, be quiet
    printf("Enter lines. Full screen or empty line clears screen.\n"
           "Supports UTF-8. CTRL-D for exit.\n");
  }*/

  /*char line[1024];
  while (fgets(line, sizeof(line), stdin)) {
    const size_t last = strlen(line);
    if (last > 0) line[last - 1] = '\0';  // remove newline.
    bool line_empty = strlen(line) == 0;
    if ((y + font.height() > canvas->height()) || line_empty) {
      canvas->Clear();
      y = y_orig;
    }
    if (line_empty)
      continue;*/
  struct tm *tm_struct = localtime(time(NULL));
  int hour = tm_struct->tm_hour;
  int minute = tm_struct->tm_min;
  char curtime[5];
  sprintf(curtime, "%d:%d", hour, minute);

    rgb_matrix::DrawText(canvas, font, x, y + font.baseline(), color, curtime);
	sleep(10);
/*    y += font.height();
  }*/

  // Finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
