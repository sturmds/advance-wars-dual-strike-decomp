#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_unit.h"
#include "game/play_records.h"
#include "game/unit_stats.h"
#include "game/screen_other_map.h"

extern PlayRecords gPlayRecords;

s32 HwDiv32(s32 numerator, s32 denominator);
BOOL Player_IsInGame(int player);
int Unit_GetChargeForHpLost(int player, int hpLost, int unitType);
void Player_AddPowerCharge(int player, int slot, int amount);
int Players_AnyHumanInGame(void);
void CurrentPlayer_SyncState(void);
void MovementCost(void);
BOOL SceneMode_IsCurrent(int arg);
void func_020c4018(void);
void CurrentPlayer_LoadPalettes(void);

/* HP as the map shows it: 0-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

#define GAME (data_027c027c->unk_48[data_027c0284])
#define OWN_MAP ((&data_027c0288.unk_00)[data_027c0284])
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

/* Two-map battles: for each player still in the game, the shown HP of their
 * units on the other map becomes power charge for both COs on this map, and
 * the other map's front CO (id and meter) becomes the back CO here. Then this
 * map starts again at player 1: gPlayRecords.unk_380 goes up as in
 * Turn_AdvanceToNextPlayer and the per-turn setup runs. Only reached through
 * a function pointer; guess: it runs when the other map's battle ends. */
void TwoMaps_MergeOtherFront(void) {
  Unk_027c0280* inner0280;
  int charge;
  int player;
  u8 frontId;
  u32 frontCharge;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_02080440* inner027c;
  int i;
  MapUnit* unit;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(OTHER_MAP);
  saved027c = data_027c027c;
  Screen_SetCurrent(OTHER_MAP);
  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player & 0xff)) {
      continue;
    }
    frontId = GAME->players[player].front.id;
    charge = 0;
    frontCharge = GAME->players[player].front.charge;
    for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
         i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
      unit = &GAME->units[i];
      if (unit->type != UNIT_NONE) {
        charge += Unit_GetChargeForHpLost(player, SHOWN_HP(unit), unit->type);
      }
    }
    inner0280 = data_027c0280;
    Screen_SetCurrentGfx(OWN_MAP);
    inner027c = data_027c027c;
    Screen_SetCurrent(OWN_MAP);
    GAME->players[player].back.id = frontId;
    GAME->players[player].back.charge = frontCharge;
    Player_AddPowerCharge(player, 0, charge);
    Player_AddPowerCharge(player, 1, charge);
    Screen_SetCurrent(inner027c);
    Screen_SetCurrentGfx(inner0280);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(OWN_MAP);
  saved027c = data_027c027c;
  Screen_SetCurrent(OWN_MAP);
  GAME->currentPlayer = 1;
  if (Players_AnyHumanInGame() && gPlayRecords.unk_380 < 999999) {
    gPlayRecords.unk_380++;
  }
  CurrentPlayer_SyncState();
  MovementCost();
  if (SceneMode_IsCurrent(4)) {
    func_020c4018();
    CurrentPlayer_LoadPalettes();
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
