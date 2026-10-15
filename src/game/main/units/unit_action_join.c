#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/math.h"

extern u32 gLinkPlay;

int Unit_GetValuePerHp(int playerIndex, int unitType);
void Player_AddFunds(int playerIndex, int amount);
void func_020c73b4(int player, int unitType);
void Unit_SubtractMoveFuel(
    MapUnit* unit,
    int tileCost); /* takes the fuel used for the move off the unit */
void ActingUnit_StartAtDest(void);

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* Unit action 10: the acting unit joins the unit on its destination tile.
 * HP are added up to 10 and the excess is paid back as funds. Ammo and fuel
 * are added up to the type's maximum. The other unit is removed. */
void UnitAction_Join(void) {
  u8 shownHp;
  u8 sum;
  MapUnit* other;

  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unitsJoined < 999999) {
      gPlayRecords.unitsJoined++;
    }
  }

  /* the unit already standing on the destination tile */
  other =
      &MAP_STATE
           ->units[MAP_STATE->mapTiles
                       ->unk_508[MAP_STATE->actingUnitDestX +
                                 MAP_STATE->mapTiles
                                     ->rowStart[MAP_STATE->actingUnitDestY]]];
  shownHp = (SHOWN_HP(MAP_STATE->actingUnit) + SHOWN_HP(other));
  if (shownHp > 10) {
    /* shown HP over 10 are paid back as funds */
    Player_AddFunds(
        MAP_STATE->currentPlayer,
        (shownHp - 10) * Unit_GetValuePerHp(MAP_STATE->currentPlayer,
                                            MAP_STATE->actingUnit->type));
    shownHp = 10;
  }
  MAP_STATE->actingUnit->hp = shownHp * 10;
  MAP_STATE->actingUnit->captureProgress = other->captureProgress;

  sum = other->ammo + MAP_STATE->actingUnit->ammo;
  if (sum > Ov000_gUnitStats[MAP_STATE->actingUnit->type].ammo) {
    MAP_STATE->actingUnit->ammo =
        Ov000_gUnitStats[MAP_STATE->actingUnit->type].ammo;
  } else {
    MAP_STATE->actingUnit->ammo = sum;
  }

  if (gLinkPlay == 0 && MAP_STATE->unk_1c4->fog == 0 &&
      MAP_STATE->unk_1c4->weather != WEATHER_RAIN) {
    Unit_SubtractMoveFuel(MAP_STATE->actingUnit, MAP_STATE->unk_46);
    MAP_STATE->unk_46 = 0;
  }

  sum = other->fuel + MAP_STATE->actingUnit->fuel;
  if (sum > Ov000_gUnitStats[MAP_STATE->actingUnit->type].fuel) {
    MAP_STATE->actingUnit->fuel =
        Ov000_gUnitStats[MAP_STATE->actingUnit->type].fuel;
  } else {
    MAP_STATE->actingUnit->fuel = sum;
  }

  other->type = UNIT_NONE;
  ActingUnit_StartAtDest();
  func_020c73b4(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
}
