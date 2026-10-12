#include "types.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

int Unit_GetValuePerHp(int playerIndex, int unitType);
int AiSite_PickPortAgainstSubs(void);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 4. Same checks and action as AiTurn_TryBuildCruiser (step 5)
 * with a different factory pick, AiSite_PickPortAgainstSubs. How the two picks differ is not
 * known, so the name stays. Returns 1 when it queued an action. */
BOOL func_02091c54(void) {
  int index;

  if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_SEA)) {
    return 0;
  }
  if (gAiProfile->units[UNIT_CRUISER - 1].unk_00 == 0) {
    return 0;
  }
  if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_CRUISER) * 10) >
      STATE->players[STATE->currentPlayer].funds) {
    return 0;
  }
  index = AiSite_PickPortAgainstSubs();
  if (index == -1) {
    return 0;
  }
  gUnitAction.target = UNIT_CRUISER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
