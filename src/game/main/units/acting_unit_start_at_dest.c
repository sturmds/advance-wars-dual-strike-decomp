#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets the acting unit's start tile to its destination, so it counts as not moved. */
void ActingUnit_StartAtDest(void) {
  STATE->actingUnitStartX = STATE->actingUnitDestX;
  STATE->actingUnitStartY = STATE->actingUnitDestY;
}
