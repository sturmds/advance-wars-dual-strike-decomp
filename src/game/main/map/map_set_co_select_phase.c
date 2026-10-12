#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Sets coSelectPhase; 1 turns off the Scout and Eagle Eye vision bonus
 * (Map_IsCoSelectPhase, VisionIncrease). */
void Map_SetCoSelectPhase(u8 value) {
  data_027c027c->unk_48[data_027c0284]->coSelectPhase = value;
}
