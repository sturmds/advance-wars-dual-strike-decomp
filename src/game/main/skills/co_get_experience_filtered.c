#include "types.h"

int Co_GetArmyExperience(int army, int coId);
extern u32 gLinkPlay;

int Co_GetExperienceFiltered(int coId, int army) {
  /* Without gLinkPlay, every army reads the shared CO experience (army 0). */
  if (gLinkPlay == 0) {
    army = 0;
  }
  return Co_GetArmyExperience(army, coId & 0x7f);
}
