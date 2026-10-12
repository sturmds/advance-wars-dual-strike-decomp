#include "sdk/snd_seq.h"

/* Attack multipliers for the settings 109-127, fastest first. */
extern u8 gSndAttackTable[19];

/* Stores the per-tick multiplier for a 0-127 attack setting: 127 is instant, lower settings rise slower. */
void SndExChannel_SetAttack(SndExChannel* channel, int attack) {
  if (attack < 109) {
    channel->attack = 255 - attack;
  } else {
    channel->attack = gSndAttackTable[127 - attack];
  }
}
