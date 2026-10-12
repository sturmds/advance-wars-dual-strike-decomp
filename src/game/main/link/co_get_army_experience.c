#include "types.h"
#include "game/link_sync_task.h"

extern LinkCoRecord data_0229deec[4][28];

u32 Co_GetExperience(u32 co);

/* Experience of a CO for a link player (army 1-4); army 0 reads the saved value. */
u32 Co_GetArmyExperience(int army, int coId) {
  if (army == 0) {
    return Co_GetExperience(coId & 0x7f);
  }
  return data_0229deec[army - 1][coId & 0x7f].experience;
}
