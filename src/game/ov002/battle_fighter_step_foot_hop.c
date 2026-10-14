#include "types.h"
#include "game/battle_fighter_sprites.h"
#include "game/battle_fighter.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"
#include "game/BattleSpriteTask.h"
#include "game/BattleCellTask.h"
#include "game/task.h"

void BattleCellTask_Update(void);
void BattleScreenTask_Init(void);
void Battle_TaskUpdate(void);

static s8 data_ov002_02369610[12] = {
#include "data/ov002/data_ov002_02369610.inc.c"
};
/* declared `u8 data_ov002_0236961c[]` in battle_reset_screen.c: relocation at data_ov002_0236961c+0xc is not on a 4-byte field */
u32 data_ov002_0236961c[8] = {
#include "data/ov002/data_ov002_0236961c.inc.c"
};
TaskScriptCmd data_ov002_0236963c[5] = {
#include "data/ov002/data_ov002_0236963c.inc.c"
};

/* One frame of the hop of an infantry or mech side: moves the fighter by the next pair of
 * offsets in data_ov002_02369610 (6 frames) and puts its place into its first sprite task. */
void BattleFighter_StepFootHop(BattleCellTask* fighter) {
  if (!fighter->flag3) {
    return;
  }
  switch (BATTLE_SIDE_UNIT_TYPE(fighter->side)) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
    case 27:
    case 28:
      break;
    default:
      return;
  }
  if (fighter->unk_57 == 6) {
    fighter->flag3 = 0;
    return;
  }
  fighter->posX += data_ov002_02369610[fighter->unk_57 * 2];
  fighter->posY += data_ov002_02369610[fighter->unk_57 * 2 + 1];
  fighter->pos2X += data_ov002_02369610[fighter->unk_57 * 2];
  fighter->pos2Y += data_ov002_02369610[fighter->unk_57 * 2 + 1];
  BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_3c = fighter->posX;
  BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_3e = fighter->posY;
  BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_42 = fighter->pos2X;
  BATTLE_FIGHTER_ENTRY(fighter).sprites[0]->unk_44 = fighter->pos2Y;
  fighter->unk_57++;
}
