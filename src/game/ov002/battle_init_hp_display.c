#include "types.h"
#include "game/battle_scene.h"
#include "game/math.h"

void Battle_PickHpDrainProfiles(void);
void Battle_ShuffleCellOrders(void);
void Battle_InitHpCells(int side, int shownHpBefore, int shownHpAfter);
void Battle_BuildHitLists(void);
void func_ov002_023552d0(void);
void func_ov002_023549c8(void);

/* HP as shown on screen: 1 to 10, rounded up. */
#define SHOWN_HP(hp) ((hp) == 0 ? 0 : HwDiv32((hp) - 1, 10) + 1)

#define SIDE(s) (Ov002_gBattleScene->sides[s])

/* Sets up the HP part of the battle screen: drain profiles, then the HP cells of each side
 * from the shown HP before and after the fight. */
void Battle_InitHpDisplay(void) {
  u32 shownAfter;
  u32 shownBefore;

  Battle_PickHpDrainProfiles();
  Battle_ShuffleCellOrders();
  shownAfter = SHOWN_HP(SIDE(0).hpAfter);
  shownBefore = SHOWN_HP(SIDE(0).hpBefore);
  Battle_InitHpCells(0, shownBefore, shownAfter);
  shownAfter = SHOWN_HP(SIDE(1).hpAfter);
  shownBefore = SHOWN_HP(SIDE(1).hpBefore);
  Battle_InitHpCells(1, shownBefore, shownAfter);
  Battle_BuildHitLists();
  func_ov002_023552d0();
  func_ov002_023549c8();
}
