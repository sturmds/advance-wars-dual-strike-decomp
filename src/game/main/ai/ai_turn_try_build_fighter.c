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
int Ai_HasHealthyFighter(void);
int Ai_EnemyHasAirUnit(void);
void Ai_BuildFighterThreatMaps(void);
int AiSite_PickAirport(void);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 3. Tries to start building a Fighter: needs an airport among
 * the current player's factories, the profile's flag for Fighters set, the
 * funds for one and a yes from Ai_HasHealthyFighter and Ai_EnemyHasAirUnit (not decompiled;
 * the first must not return 1, the second must be nonzero). AiSite_PickAirport
 * picks the factory (-1: none). The name is a guess from the unit type and the
 * checks. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildFighter(void) {
  int index;

  if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_AIR)) {
    return 0;
  }
  if (gAiProfile->units[UNIT_FIGHTER - 1].unk_00 == 0) {
    return 0;
  }
  if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_FIGHTER) * 10) >
      STATE->players[STATE->currentPlayer].funds) {
    return 0;
  }
  if (Ai_HasHealthyFighter() == 1) {
    return 0;
  }
  if (Ai_EnemyHasAirUnit() == 0) {
    return 0;
  }
  Ai_BuildFighterThreatMaps();
  index = AiSite_PickAirport();
  if (index == -1) {
    return 0;
  }
  gUnitAction.target = UNIT_FIGHTER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
