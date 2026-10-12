#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

void Sound_PlaySe(u32 soundId);
BOOL Battle_IsSideAir(int side);
int Battle_IsSideSea(int side);

/* The sound a fighter makes when its own sound table has none: one for
 * oozium, one for air units, one for Battle_IsSideSea (looks like the
 * ships), one for the rest. */
void Battle_PlayDefaultFighterSe(int side) {
  if (BATTLE_SIDE_UNIT_TYPE(side) == UNIT_OOZIUM) {
    Sound_PlaySe(0x1c0002);
  } else if (Battle_IsSideAir(side) == 1) {
    Sound_PlaySe(0x2000d);
  } else if (Battle_IsSideSea(side) == 1) {
    Sound_PlaySe(0x2000e);
  } else {
    Sound_PlaySe(0x2000c);
  }
}
