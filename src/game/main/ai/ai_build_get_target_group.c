#include "types.h"
#include "game/ai_build_state.h"
#include "game/unit_stats.h"

/* Target group (TARGET_GROUP_*) of a unit type, for AiBuild_WeighUnitTypes. */
int AiBuild_GetTargetGroup(u32 unitType) {
  switch (unitType) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      return TARGET_GROUP_FOOT;
    case UNIT_RECON:
    case UNIT_APC:
    case UNIT_PIPERUNNER:
    case UNIT_ARTILLERY:
    case UNIT_ROCKETS:
    case UNIT_MISSILES:
      return TARGET_GROUP_LIGHT;
    case UNIT_MEDIUM_TANK:
    case UNIT_TANK:
    case UNIT_NEOTANK:
    case UNIT_ANTI_AIR:
      return TARGET_GROUP_TANK;
    case UNIT_STEALTH:
    case UNIT_BLACK_BOMB:
    case UNIT_FIGHTER:
    case UNIT_BOMBER:
      return TARGET_GROUP_PLANE;
    case UNIT_B_COPTER:
    case UNIT_T_COPTER:
      return TARGET_GROUP_COPTER;
    case UNIT_BATTLESHIP:
      return TARGET_GROUP_BATTLESHIP;
    case UNIT_CRUISER:
      return TARGET_GROUP_CRUISER;
    case UNIT_BLACK_BOAT:
    case UNIT_LANDER:
    case UNIT_CARRIER:
      return TARGET_GROUP_TRANSPORT;
    case UNIT_SUB:
      return TARGET_GROUP_SUB;
    case UNIT_OOZIUM:
    default:
      return TARGET_GROUP_NONE;
  }
}
