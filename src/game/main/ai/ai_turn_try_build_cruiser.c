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
int AiSite_PickPortForCruiser(void);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 5. Tries to start building a Cruiser: needs a port among
 * the current player's factories, the profile's flag for Cruisers set and the
 * funds for one. AiSite_PickPortForCruiser picks the factory (-1: none). The name is a
 * guess from the unit type and the checks. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildCruiser(void) {
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
  index = AiSite_PickPortForCruiser();
  if (index == -1) {
    return 0;
  }
  gUnitAction.target = UNIT_CRUISER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
