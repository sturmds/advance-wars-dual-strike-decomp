#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

s32 HwDiv32(s32 numerator, s32 denominator);
int Unit_GetValuePerHp(int playerIndex, int unitType);
void Player_AddFunds(int playerIndex, int amount);
void func_020c73b4(int player, int unitType);
void Unit_SubtractMoveFuel(
    MapUnit* unit,
    int tileCost); /* takes the fuel used for the move off the unit */
void ActingUnit_StartAtDest(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* Unit action 10: the acting unit joins the unit on its destination tile.
 * HP are added up to 10 and the excess is paid back as funds. Ammo and fuel
 * are added up to the type's maximum. The other unit is removed. */
void UnitAction_Join(void) {
  u8 shownHp;
  u8 sum;
  MapUnit* other;

  if (GAME->players[GAME->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unitsJoined < 999999) {
      gPlayRecords.unitsJoined++;
    }
  }

  /* the unit already standing on the destination tile */
  other = &GAME->units[GAME->mapTiles->unk_508
                           [GAME->actingUnitDestX +
                            GAME->mapTiles->rowStart[GAME->actingUnitDestY]]];
  shownHp = (SHOWN_HP(GAME->actingUnit) + SHOWN_HP(other));
  if (shownHp > 10) {
    /* shown HP over 10 are paid back as funds */
    Player_AddFunds(
        GAME->currentPlayer,
        (shownHp - 10) *
            Unit_GetValuePerHp(GAME->currentPlayer, GAME->actingUnit->type));
    shownHp = 10;
  }
  GAME->actingUnit->hp = shownHp * 10;
  GAME->actingUnit->captureProgress = other->captureProgress;

  sum = other->ammo + GAME->actingUnit->ammo;
  if (sum > Ov000_gUnitStats[GAME->actingUnit->type].ammo) {
    GAME->actingUnit->ammo = Ov000_gUnitStats[GAME->actingUnit->type].ammo;
  } else {
    GAME->actingUnit->ammo = sum;
  }

  if (gLinkPlay == 0 && GAME->unk_1c4->fog == 0 &&
      GAME->unk_1c4->weather != WEATHER_RAIN) {
    Unit_SubtractMoveFuel(GAME->actingUnit, GAME->unk_46);
    GAME->unk_46 = 0;
  }

  sum = other->fuel + GAME->actingUnit->fuel;
  if (sum > Ov000_gUnitStats[GAME->actingUnit->type].fuel) {
    GAME->actingUnit->fuel = Ov000_gUnitStats[GAME->actingUnit->type].fuel;
  } else {
    GAME->actingUnit->fuel = sum;
  }

  other->type = UNIT_NONE;
  ActingUnit_StartAtDest();
  func_020c73b4(GAME->currentPlayer, GAME->actingUnit->type);
}
