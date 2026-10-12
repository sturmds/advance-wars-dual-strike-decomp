#include "types.h"
#include "game/menu_state.h"
#include "game/BattleSideData.h"

extern MenuState gMenuState;

int Battle_GetAirGroundMatchup(int side);

/* Needed to match: the 0x27c is added to the symbol at run time, and every case
 * loads the whole chain again. */
#define SIDE_DATA(side) \
  (((BattleSideData**)((u8*)&gMenuState + (side) * 4))[0x27c / 4])

/* A 4-bit value from the side's data, picked by how the two units differ in altitude. */
int Battle_GetSideMatchupValue(int side) {
  switch (Battle_GetAirGroundMatchup(side)) {
    case 0:
      return SIDE_DATA(side)->unk_10->sameAltitude;
    case 1:
      return SIDE_DATA(side)->unk_10->groundVsAir;
    case 2:
      return SIDE_DATA(side)->unk_10->airVsGround;
    default:
      return SIDE_DATA(side)->unk_10->sameAltitude;
  }
}
