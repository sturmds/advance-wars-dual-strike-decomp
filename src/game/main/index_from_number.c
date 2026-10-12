#include "types.h"

/* Turns a 1-based number into a 0-based index; 0 counts as 1, so it gives 0. */
int Index_FromNumber(int number) {
  if (number == 0) {
    number = 1;
  }
  return number - 1;
}
