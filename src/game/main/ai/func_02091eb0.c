#include "types.h"
#include "game/ai_profile.h"
#include "game/ai_site_group.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s8 data_0229e5c0[][12];

#define STATE (data_027c027c->unk_48[data_027c0284])

int Unit_GetValuePerHp(int playerIndex, int unitType);
int AiBuildSlot_PickSafeSite(void);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 1. Like AiTurn_TryBuildFighter, for unit type 14
 * (UNIT_ANTI_AIR), but gated on the sea bit of domainsOwn, which does not fit
 * that type, and it also clears needAntiAir in the site group of
 * AiBuildSlot_PickSafeSite's pick. Both are unexplained, so the name stays.
 * Returns 1 when it queued an action. */
BOOL func_02091eb0(void) {
  int index;
  AiSiteGroup* record;

  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return 0;
  }
  if (gAiProfile->units[UNIT_ANTI_AIR - 1].unk_00 == 0) {
    return 0;
  }
  if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_ANTI_AIR) * 10) >
      STATE->players[STATE->currentPlayer].funds) {
    return 0;
  }
  index = AiBuildSlot_PickSafeSite();
  if (index == -1) {
    return 0;
  }
  record = &gAiSiteGroups[data_0229e5c0[index][0]];
  record->needAntiAir = 0;
  gUnitAction.target = UNIT_ANTI_AIR;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
