#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"
#include "game/terrain.h"

/* A side as the code reads it: BattleScene.sides[] with the unit type one byte before it
 * (see BATTLE_SIDE_UNIT_TYPE). */
typedef struct SideRecord {
  u8 unitType;
  u8 hpBefore;
  u8 hpAfter;
  u8 terrain;
  u8 pad_04[2];
  u8 usesSecondary;
  u8 counters;
  u8 dived;
} SideRecord;

#define SIDE(offset) ((SideRecord*)((u8*)Ov002_gBattleScene + (offset) + 3))

/* Unit types the battle animation draws as their own sprites. */
#define BATTLE_UNIT_INFANTRY_IN_RIVER 27
#define BATTLE_UNIT_MECH_IN_RIVER 28
#define BATTLE_UNIT_ANTI_AIR_COUNTER 29
#define BATTLE_UNIT_PIPERUNNER_COUNTER 30
#define BATTLE_UNIT_STEALTH_COUNTER 31
#define BATTLE_UNIT_SUB_DIVED 32

int Battle_GetAirGroundMatchup(int side);

/* Swaps the unit type of each side for the variant the animation draws: infantry and mechs in
 * a river, anti-air, piperunners and stealths that hit back, and dived subs. */
void Battle_PickSideVariants(void) {
  int side;
  int off;

  for (side = 0, off = 0; side < 2; side++, off += 12) {
    switch (SIDE(off)->unitType) {
      case UNIT_INFANTRY:
        if (SIDE(off)->terrain == TERRAIN_RIVER) {
          SIDE(off)->unitType = BATTLE_UNIT_INFANTRY_IN_RIVER;
        }
        break;
      case UNIT_MECH:
        if (SIDE(off)->terrain == TERRAIN_RIVER) {
          SIDE(off)->unitType = BATTLE_UNIT_MECH_IN_RIVER;
        }
        break;
      case UNIT_ANTI_AIR:
        if (SIDE(off)->counters != 0 && Battle_GetAirGroundMatchup(side) != 1) {
          SIDE(off)->unitType = BATTLE_UNIT_ANTI_AIR_COUNTER;
        }
        break;
      case UNIT_PIPERUNNER:
        if (SIDE(off)->counters != 0 && Battle_GetAirGroundMatchup(side) != 1) {
          SIDE(off)->unitType = BATTLE_UNIT_PIPERUNNER_COUNTER;
        }
        break;
      case UNIT_STEALTH:
        if (SIDE(off)->counters != 0 && Battle_GetAirGroundMatchup(side) == 2) {
          SIDE(off)->unitType = BATTLE_UNIT_STEALTH_COUNTER;
        }
        break;
      case UNIT_SUB:
        if (SIDE(off)->dived != 0) {
          SIDE(off)->unitType = BATTLE_UNIT_SUB_DIVED;
        }
        break;
    }
  }
}
