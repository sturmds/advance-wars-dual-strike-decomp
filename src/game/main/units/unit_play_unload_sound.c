#include "types.h"
#include "game/unit_stats.h"

void Sound_PlaySe(u32 soundId);

/* Plays the sound of a unit stepping off a transport. Only foot soldiers
 * (Infantry and Mech) have one. */
void UnitType_PlayUnloadSound(int unitType) {
  switch (unitType) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      Sound_PlaySe(149);
      break;
  }
}
