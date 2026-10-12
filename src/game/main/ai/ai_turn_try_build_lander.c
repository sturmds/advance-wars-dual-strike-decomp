#include "types.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

int Ai_CountUnitsOfType(int type);
int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL AiProfile_HasBuildFlag(int mask);
int AiTurn_TryBuildLanderNearSite(void);
int AiSite_PickForLander(int arg);
int AiSite_PickFirst(int arg);
int Ai_CountVehiclesWantingTransport(void);
int Ai_CountFootWantingTransport(void);
int Ai_CountEmptyTransports(int type);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 8, second sub-step. Tries to start building a Lander:
 * needs bit 3 of the profile byte that AiProfile_HasBuildFlag tests and the funds for
 * one. AiTurn_TryBuildLanderNearSite runs first and ends the step when it returns 1.
 * AiSite_PickForLander(3), then AiSite_PickFirst(3), pick the factory (-1: none).
 * Ai_CountVehiclesWantingTransport counts the player's vehicles (type above Mech) with
 * MapUnit.transportWanted 2, Ai_CountFootWantingTransport the foot units with it, and
 * Ai_CountEmptyTransports the player's units of a type with nothing in cargo[0].
 * With no vehicle waiting nothing is built. When Landers exist already, the
 * new one needs four waiting units per empty Lander, itself included,
 * counting foot units only past four per empty Black Boat. The name is a
 * guess from the unit type. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildLander(void) {
  Unk_02080440_48* state;
  int vehiclesWaiting;
  int footWaiting;
  int index;
  int emptyLandersAfter;
  int count;

  if (AiProfile_HasBuildFlag(8) == 0) {
    return 0;
  }
  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_LANDER) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (AiTurn_TryBuildLanderNearSite() == 1) {
    return 1;
  }
  index = AiSite_PickForLander(3);
  if (index == -1) {
    index = AiSite_PickFirst(3);
    if (index == -1) {
      return 0;
    }
  }
  count = Ai_CountUnitsOfType(UNIT_LANDER);
  vehiclesWaiting = Ai_CountVehiclesWantingTransport();
  if (vehiclesWaiting == 0) {
    return 0;
  }
  if (count != 0) {
    emptyLandersAfter = Ai_CountEmptyTransports(UNIT_LANDER) + 1;
    count = Ai_CountEmptyTransports(UNIT_BLACK_BOAT);
    footWaiting = Ai_CountFootWantingTransport() - count * 4;
    if (footWaiting < 0) {
      footWaiting = 0;
    }
    if (emptyLandersAfter * 4 > vehiclesWaiting + footWaiting) {
      return 0;
    }
  }
  gUnitAction.target = UNIT_LANDER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
