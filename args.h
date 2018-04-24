#ifndef ARGS_H
#define ARGS_H

#include "complex.h"

typedef struct Args
{
  int width;
  int height;
  Complex top_left;
  Complex bottom_right;
  int maxiterations;
  char* filename;
} Args;


void display_help_msg();
Args parse_args(int, char**);


#endif /* ARGS_H */
