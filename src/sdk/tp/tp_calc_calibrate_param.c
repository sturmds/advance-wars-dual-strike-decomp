#include "sdk/tp.h"
#include "sdk/cp.h"
#include "sdk/os.h"

#define TOUCH_12BIT_CEILING 4096 /* raw readings are 12 bits */
#define LCD_WIDTH 256
#define LCD_HEIGHT 192

/*
 * Builds a calibration from two reference points: the raw reading (rawX1,
 * rawY1) taken while the stylus was on screen pixel (dx1, dy1), and the same
 * for the second point. Returns 0 on success and 1 when a value is out of
 * range, the points share an axis, or a result does not fit in s16.
 */
u32 TP_CalcCalibrateParam(TPCalibrateParam* calibrate, u16 rawX1, u16 rawY1,
                          u16 dx1, u16 dy1, u16 rawX2, u16 rawY2, u16 dx2,
                          u16 dy2) {
  int enabled;
  s32 rawWidth;
  s32 dotWidth;
  s32 rawHeight;
  s32 dotHeight;
  s32 data;

  if (rawX1 >= TOUCH_12BIT_CEILING || rawY1 >= TOUCH_12BIT_CEILING ||
      rawX2 >= TOUCH_12BIT_CEILING || rawY2 >= TOUCH_12BIT_CEILING) {
    return 1;
  }
  if (dx1 >= LCD_WIDTH || dx2 >= LCD_WIDTH || dy1 >= LCD_HEIGHT ||
      dy2 >= LCD_HEIGHT) {
    return 1;
  }
  if (dx1 == dx2 || dy1 == dy2 || rawX1 == rawX2 || rawY1 == rawY2) {
    return 1;
  }

  enabled = OS_DisableInterrupts();

  rawWidth = rawX1 - rawX2;
  dotWidth = dx1 - dx2;
  CP_SetDiv32_32((u32)rawWidth << 8, (u32)dotWidth);
  rawHeight = rawY1 - rawY2;
  dotHeight = dy1 - dy2;
  CP_WaitDiv();
  data = CP_GetDivResult32();

  CP_SetDiv32_32((u32)rawHeight << 8, (u32)dotHeight);
  if (data >= 0x8000 || data < -0x8000) {
    OS_RestoreInterrupts(enabled);
    return 1;
  }
  calibrate->xDotSize = data;
  /* After the s16 cast the range check below is never true; the original has both. */
  data =
      (s16)((((rawX1 + rawX2) << 8) - (dx1 + dx2) * calibrate->xDotSize) >> 7);
  if (data >= 0x8000 || data < -0x8000) {
    OS_RestoreInterrupts(enabled);
    return 1;
  }
  calibrate->x0 = data;

  CP_WaitDiv();
  data = CP_GetDivResult32();
  OS_RestoreInterrupts(enabled);
  if (data >= 0x8000 || data < -0x8000) {
    return 1;
  }
  calibrate->yDotSize = data;
  data =
      (s16)((((rawY1 + rawY2) << 8) - (dy1 + dy2) * calibrate->yDotSize) >> 7);
  if (data >= 0x8000 || data < -0x8000) {
    return 1;
  }
  calibrate->y0 = data;
  return 0;
}
