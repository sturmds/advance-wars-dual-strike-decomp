#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/battle_scene.h"

#define MENU ((MenuState*)&gMenuState)

/* Vertical offset of a cell's objects, scaled by the task's scale: the layout offset of
 * the weapon plus the one of the weapon and the army style. Needed to match:
 * BattleSide.usesSecondary and armyStyle are read as bytes 9 and 13 counted from the scene's
 * 12 bytes of this side. */
int BattleCellTask_GetOffsetY(BattleCellTask* task) {
  BattleLayout* layout =
      (BattleLayout*)MENU->battle.unk_27c[task->side]->cellPositions;
  u8* sideBytes = (u8*)Ov002_gBattleScene + task->side * sizeof(BattleSide);
  int weapon = sideBytes[9];
  BattleOffset* styleOffset =
      &layout->weaponStyleOffsets[weapon][sideBytes[13] - 1];
  BattleOffset* offset = &layout->weaponOffsets[weapon];

  return (offset->y + styleOffset->y) * task->scale >> 8;
}
