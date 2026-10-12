#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* TRUE when the unit slot (an index into the units array) lies in the
 * current player's block of 64 slots. */
BOOL Unit_IsCurrentPlayers(int unitSlot) {
  return (unitSlot & 0xc0) ==
         data_027c027c->unk_48[data_027c0284]->currentUnitBase;
}
