#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

/* Skills used when CO effects are off; it lies inside gCoTable. */
extern u32 data_02153af4[];

/* The four skill words of a CO in a power state: CoPowerBlock.skills1 on,
 * 16 bytes. */
u32* GetCoPowerSkills(int co, int powerState) {
  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return data_02153af4;
  }
  return (u32*)&gCoTable[co].blocks[powerState].skills1;
}
