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
int AiTurn_TryBuildBlackBoatNearSite(void);
int AiSite_PickNearestFoot(int arg);
int AiSite_PickFirst(int arg);
int Ai_CountVehiclesWantingTransport(void);
int Ai_CountFootWantingTransport(void);
int Ai_CountEmptyTransports(int type);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 8, third sub-step. Tries to start building a Black Boat
 * to carry foot units, the way AiTurn_TryBuildLander does for Landers:
 * needs bit 2 of the profile byte that AiProfile_HasBuildFlag tests and the funds
 * for one. AiTurn_TryBuildBlackBoatNearSite runs first and ends the step when it returns 1.
 * AiSite_PickNearestFoot(3), then AiSite_PickFirst(3), pick the factory (-1: none).
 * With no foot unit waiting (Ai_CountFootWantingTransport) nothing is built. When the
 * player has a Lander or a Black Boat, the new one needs four waiting foot
 * units per empty Black Boat, itself included, and four more per empty
 * Lander left over after one per four waiting vehicles. The name is a
 * guess. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildBlackBoatTransport(void) {
  Unk_02080440_48* state;
  int spareLanders;
  int footWaiting;
  int emptyBoatsAfter;
  int index;
  int boats;
  int landers;

  if (AiProfile_HasBuildFlag(4) == 0) {
    return 0;
  }
  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_BLACK_BOAT) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (AiTurn_TryBuildBlackBoatNearSite() == 1) {
    return 1;
  }
  index = AiSite_PickNearestFoot(3);
  if (index == -1) {
    index = AiSite_PickFirst(3);
    if (index == -1) {
      return 0;
    }
  }
  landers = Ai_CountUnitsOfType(UNIT_LANDER);
  boats = Ai_CountUnitsOfType(UNIT_BLACK_BOAT);
  footWaiting = Ai_CountFootWantingTransport();
  if (footWaiting == 0) {
    return 0;
  }
  if (landers + boats != 0) {
    landers = Ai_CountEmptyTransports(UNIT_LANDER);
    emptyBoatsAfter = Ai_CountEmptyTransports(UNIT_BLACK_BOAT) + 1;
    spareLanders = landers - (Ai_CountVehiclesWantingTransport() >> 2);
    if (spareLanders < 0) {
      spareLanders = 0;
    }
    if ((emptyBoatsAfter + spareLanders) * 4 > footWaiting) {
      return 0;
    }
  }
  gUnitAction.target = UNIT_BLACK_BOAT;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
