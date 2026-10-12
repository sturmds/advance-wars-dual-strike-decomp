#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Capture rate of a CO in a power state, as 100 plus its bonus in percent.
 * 100 when coEffectsOn is clear. */
int Co_GetCapturePercent(int co, int powerState) {
  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return 100;
  }
  return gCoTable[co].blocks[powerState].captureRate + 100;
}
