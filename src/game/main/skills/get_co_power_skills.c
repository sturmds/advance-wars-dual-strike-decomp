#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

/* The four skill words of a CO in a power state: CoPowerBlock.skills1 on,
 * 16 bytes. */
u32* GetCoPowerSkills(int co, int powerState) {
  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    /* CO effects off: Andy's day-to-day skills, which are none */
    return (u32*)&gCoTable[CO_ANDY].blocks[POWER_NONE].skills1;
  }
  return (u32*)&gCoTable[co].blocks[powerState].skills1;
}
