#include "types.h"
#include "game/battle_fighter_sprites.h"
#include "game/battle_fighter.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/BattleCellTask.h"

BOOL Battle_IsSideDrawnInAir(int side);

/* One frame of a fighter that adds 1 to unk_38 and unk_3e, for 128 frames,
 * and puts its place into its three sprite tasks. Runs while the side's BattleSideData.unk_48
 * is 0 and the side passes Battle_IsSideDrawnInAir; the name is a guess from what moves. */
void BattleFighter_StepSlideMode0(BattleCellTask* fighter) {
  if (!fighter->flag3) {
    return;
  }
  if (!Battle_IsSideDrawnInAir(fighter->side)) {
    return;
  }
  if (BATTLE_SIDE_DATA(fighter->side)->unk_48 != 0) {
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
