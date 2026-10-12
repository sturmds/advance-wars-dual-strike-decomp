#include "types.h"
#include "sdk/msl.h"

double fabs(double x) {
  ((u32*)&x)[1] &= 0x7FFFFFFF;
  return x;
}
