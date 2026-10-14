#include "types.h"

static u32 gSndRandomSeed = {
#include "data/arm7/gSndRandomSeed.inc.c"
};
char data_arm7_038077c0[] = {
#include "data/arm7/data_arm7_038077c0.inc.c"
};

/* Linear congruential generator; returns the high 16 bits. */
u16 SndDrv_NextRandom(void) {
  gSndRandomSeed = gSndRandomSeed * 0x19660D + 0x3C6EF35F;
  return gSndRandomSeed >> 16;
}
