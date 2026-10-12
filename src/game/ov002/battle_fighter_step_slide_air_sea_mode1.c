#include "types.h"
#include "game/battle_fighter_sprites.h"
#include "game/battle_fighter.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/BattleCellTask.h"

BOOL Battle_IsSideAir(int side);
BOOL Battle_IsSideSea(int side);

/* Like BattleFighter_StepSlideMode0, for a plane, helicopter or boat side whose
 * BattleSideData.unk_48 is 1. */
void BattleFighter_StepSlideAirSeaMode1(BattleCellTask* fighter) {
  if (!fighter->flag3) {
    return;
  }
  if (!Battle_IsSideAir(fighter->side) && !Battle_IsSideSea(fighter->side)) {
    return;
  }
  if (BATTLE_SIDE_DATA(fighter->side)->unk_48 != 1) {
    return;
  }
  if (fighter->unk_57 == 0x80) {
    fighter->flag3 = 0;
    return;
  }
  fighter->posX++;
  fighter->pos2X++;
  BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_3c = fighter->posX;
  BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_42 = fighter->pos2X;
  if (BATTLE_FIGHTER_ENTRY(fighter).sprites[1] != 0) {
    BATTLE_FIGHTER_ENTRY(fighter).sprites[1]->unk_3c = fighter->posX;
    BATTLE_FIGHTER_ENTRY(fighter).sprites[1]->unk_42 = fighter->pos2X;
  }
  if (BATTLE_FIGHTER_ENTRY(fighter).sprites[2] != 0) {
    BATTLE_FIGHTER_ENTRY(fighter).sprites[2]->unk_3c = fighter->posX;
    BATTLE_FIGHTER_ENTRY(fighter).sprites[2]->unk_42 = fighter->pos2X;
  }
  fighter->unk_57++;
}
