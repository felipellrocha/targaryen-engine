#include "utils.h"

bool overlap(int a0, int a1, int b0, int b1) {
  return a1 >= b0 && b1 >= a0;
}

