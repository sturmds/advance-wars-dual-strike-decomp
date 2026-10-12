#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int SuperPowerCost(int player, int slot);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Needed to match: the original reads data_027c0284 and then data_027c027c
 * again instead of reusing the state pointer it already holds. */
#define RELOAD_STATE(dst)                                             \
  {                                                                   \
    int index_ = *(volatile int*)&data_027c0284;                      \
    dst = (*(Unk_02080440* volatile*)&data_027c027c)->unk_48[index_]; \
  }

/* For an AI player with a tag partner and no power on: TRUE when the front
 * CO is nearer its Super CO Power than the back CO is to its own (in meter
 * charge still missing). Player.tagPowerHalf, when set, decides instead:
 * TAG_POWER_FRONT gives TRUE, TAG_POWER_BACK FALSE. AiEndActions_Start
 * passes the result to AiUnit_GetRepairValue. */
BOOL AiPlayer_FrontCoNearerToSuper(void) {
  Unk_02080440_48* state;
  int player;
  u32 hasPartner; /* needed to match: a u32 keeps the compare with FALSE */
  int current;
  int frontCost;
  int backCost;

  RELOAD_STATE(state);
  player = state->currentPlayer;
  if (state->players[player].front.powerState != 0) {
    return FALSE;
  }
  hasPartner = state->players[player].back.id != 0;
  if (hasPartner == FALSE) {
    return FALSE;
  }
  if (state->players[player].tagPowerHalf == TAG_POWER_FRONT) {
    return TRUE;
  }
  if (state->players[player].tagPowerHalf == TAG_POWER_BACK) {
    return FALSE;
  }
  if (state->players[player].tagPowerHalf == 0) {
    RELOAD_STATE(state);
    current = state->currentPlayer;
    frontCost = SuperPowerCost(current, 0);
    backCost = SuperPowerCost(player, 1);
    if ((u32)(frontCost - state->players[state->currentPlayer].front.charge) <
        (u32)(backCost - state->players[current].back.charge)) {
      return TRUE;
    }
  }
  return FALSE;
}
