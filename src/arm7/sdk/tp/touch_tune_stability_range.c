#include "types.h"
#include "sdk/tp.h"

#define RANGE_MAX 35
#define RANGE_ADJUST_COUNT 4 /* samples in a row before the range moves */

static u8 tpInvalidCount;
static u8 tpValidCount;
TouchSamplerState tpWork;

/* Widens the stability range after RANGE_ADJUST_COUNT invalid samples in a
 * row, and narrows it again (not below rangeMin) after as many valid samples
 * whose spread is under half the range. */
void Touch_TuneStabilityRange(SPITpData* sample, u16 density) {
  if (!sample->e.touch) {
    tpInvalidCount = 0;
    tpValidCount = 0;
    return;
  }

  if (sample->e.validity) {
    tpValidCount = 0;
    if (++tpInvalidCount >= RANGE_ADJUST_COUNT) {
      tpInvalidCount = 0;
      if (tpWork.range < RANGE_MAX) {
        tpWork.range++;
      }
    }
  } else {
    tpInvalidCount = 0;
    if (density >= (tpWork.range >> 1)) {
      tpValidCount = 0;
      return;
    }
    if (++tpValidCount >= RANGE_ADJUST_COUNT) {
      tpValidCount = 0;
      if (tpWork.range > tpWork.rangeMin) {
        tpWork.range--;
        /* one more invalid sample widens it back */
        tpInvalidCount = RANGE_ADJUST_COUNT - 1;
      }
    }
  }
}
