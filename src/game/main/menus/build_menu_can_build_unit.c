#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

BOOL Campaign_CanPlayerBuildUnit(int player, int unitType);
BOOL BuildMenu_CanBuildUnitOutsideCampaign(int player, int unitType);

/* TRUE when the player may build the unit type. The campaign limits the
 * list per mission; the other game modes allow all but Oozium. */
BOOL BuildMenu_CanBuildUnit(int player, int unitType) {
  switch (GAME->gameMode) {
    case GAME_MODE_CAMPAIGN:
      return Campaign_CanPlayerBuildUnit(player, unitType);
    case GAME_MODE_WAR_ROOM:
    case GAME_MODE_VERSUS:
    default:
      return BuildMenu_CanBuildUnitOutsideCampaign(player, unitType);
  }
}
