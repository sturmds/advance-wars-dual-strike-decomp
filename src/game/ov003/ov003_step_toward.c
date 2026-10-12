#include "types.h"

int Math_Sqrt32(int squared);
int HwDiv32(int numerator, int denominator);

/* Sets *outX and *outY to the move of the given length from (x1, y1) toward (x2, y2). Both are 0 when the points match. */
void Ov003_StepToward(int x1, int y1, int x2, int y2, int length, int* outX,
                      int* outY) {
  int dx = (x1 - x2 < 0 ? -(x1 - x2) : x1 - x2) << 7;
  int dy = (y1 - y2 < 0 ? -(y1 - y2) : y1 - y2) << 7;
  int distance;
  int stepX;
  int stepY;

  if (dx == 0 && dy == 0) {
    *outX = 0;
    *outY = 0;
    return;
  }
  distance = Math_Sqrt32(dx * dx + dy * dy);
  stepX = HwDiv32(length * dx, distance);
  stepY = HwDiv32(length * dy, distance);
  if (x1 < x2) {
    *outX = stepX;
  } else {
    *outX = -stepX;
  }
  if (y1 < y2) {
    *outY = stepY;
  } else {
    *outY = -stepY;
  }
}
