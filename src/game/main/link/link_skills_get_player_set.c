#include "types.h"

extern u8 data_0229de6c[4][2][16];

/* The 16 skill bytes a link slot (1-4; 0: none, returns NULL) sent for one of its two COs, as
 * LinkSkillSyncTask_Update stored them. */
u8* LinkSkills_GetPlayerSet(int slot, int coIndex) {
  if (slot == 0) {
    return NULL;
  }
  return data_0229de6c[slot - 1][coIndex];
}
