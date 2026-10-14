#include "types.h"
#include "game/link_sync_task.h"
#include "game/co_data.h"

u32 Co_GetExperience(u32 co);

/* Experience of a CO for a link player (army 1-4); army 0 reads the saved value. */
u32 Co_GetArmyExperience(int army, int coId) {
  if (army == 0) {
    return Co_GetExperience(coId & CO_ID_MASK);
  }
  return data_0229deec[army - 1][coId & CO_ID_MASK].experience;
}
