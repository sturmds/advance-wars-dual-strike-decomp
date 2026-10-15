#include "types.h"
#include "game/battle_scene.h"
#include "game/Unk_0229fb30.h"
#include "game/unit_stats.h"
#include "game/math.h"

int UnitType_BestBaseDamage(int unitType, int targetType);
int UnitType_GetAttackKind(u8 unitType);
int Battle_GetSideUnitType(int side);
int Battle_IsArmyNotHuman(int army);

/* Picks the frame graphics of the battle screen into data_0229fb30.unk_03.
 * 0: nobody or both sides died, or Battle_IsArmyNotHuman returns 1 for the
 * survivor's army. 3: the survivor wiped out a full-HP unit that is not
 * indirect. Otherwise 1 or 2 by the survivor's best base damage on the loser. */
void Battle_PickFrame(void) {
  int survivor = -1;
  int loser;
  int loserHp;

  if (Ov002_gBattleScene->sides[0].hpAfter != 0) {
    if (Ov002_gBattleScene->sides[1].hpAfter == 0) {
      survivor = 0;
    }
  }
  if (Ov002_gBattleScene->sides[0].hpAfter == 0) {
    if (Ov002_gBattleScene->sides[1].hpAfter != 0) {
      survivor = 1;
    }
  }
  if (survivor == -1) {
    data_0229fb30.unk_03 = 0;
    return;
  }
  if (Battle_IsArmyNotHuman(Ov002_gBattleScene->sides[survivor].army) == 1) {
    data_0229fb30.unk_03 = 0;
    return;
  }
  loser = (survivor + 1) & 1;
  if (Ov002_gBattleScene->sides[loser].hpBefore == 0) {
    loserHp = 0;
  } else {
    loserHp = HwDiv32(Ov002_gBattleScene->sides[loser].hpBefore - 1, 10) + 1;
  }
  if (loserHp == 10 && Battle_GetSideUnitType(loser) > 2) {
    if (UnitType_GetAttackKind((u8)Battle_GetSideUnitType(loser)) !=
        ATTACK_KIND_NONE) {
      data_0229fb30.unk_03 = 3;
      return;
    }
  }
  if (UnitType_BestBaseDamage(Battle_GetSideUnitType(survivor),
                              Battle_GetSideUnitType(loser)) <= 20) {
    data_0229fb30.unk_03 = 2;
  } else {
    data_0229fb30.unk_03 = 1;
  }
}
