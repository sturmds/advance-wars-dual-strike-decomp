#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai_site_group.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern s8 data_0229e5c0[][12];

#define STATE (data_027c027c->unk_48[data_027c0284])

int Rng_Next(void);
s32 HwMod32(s32 numer, s32 denom);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int AiSite_PickForFootUnit(void);
int AiSite_PickNearestToPropertyAnyGroup(int arg);
int AiSite_PickFirst(int arg);
int Ai_CountUnitsOfAiClass(int arg);
u32 AiTurn_GetMechBuildPercent(void);
void func_020b81bc(void);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 6. Tries to start building a foot unit if funds cover an
 * Infantry. Takes the first factory that AiSite_PickForFootUnit,
 * AiSite_PickNearestToPropertyAnyGroup(1) or AiSite_PickFirst(1) return
 * (-1: none). A factory whose site group already has skipFoot set is skipped
 * when unk_08 reaches the profile's unk_08[0] + unk_08[1]. The unit is a Mech
 * when a roll of 0 to 99 is under AiTurn_GetMechBuildPercent() and funds cover 20 times its
 * value per HP, else an Infantry. The name and the meaning of the flags are
 * guesses. */
BOOL AiTurn_TryBuildFootUnit(void) {
  int index;
  u32 roll;
  AiSiteGroup* record;

  if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_INFANTRY) * 10) >
      STATE->players[STATE->currentPlayer].funds) {
    return 0;
  }
  index = AiSite_PickForFootUnit();
  if (index == -1) {
    index = AiSite_PickNearestToPropertyAnyGroup(1);
  }
  if (index == -1) {
    index = AiSite_PickFirst(1);
  }
  if (index == -1) {
    return 0;
  }
  record = &gAiSiteGroups[data_0229e5c0[index][0]];
  if (record->skipFoot == 0) {
    record->skipFoot = 1;
  } else {
    data_021619c0->unk_08 = Ai_CountUnitsOfAiClass(1);
    if (data_021619c0->unk_08 >=
        gAiProfile->unk_08[0] + gAiProfile->unk_08[1]) {
      return 0;
    }
  }
  roll = HwMod32(Rng_Next(), 100);
  if (roll < AiTurn_GetMechBuildPercent() &&
      (u32)STATE->players[STATE->currentPlayer].funds >=
          (u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_MECH) * 20)) {
    gUnitAction.target = UNIT_MECH;
  } else {
    gUnitAction.target = UNIT_INFANTRY;
  }
  func_020b81bc();
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
