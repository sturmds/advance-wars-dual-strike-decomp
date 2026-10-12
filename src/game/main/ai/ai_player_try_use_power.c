#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/co_data.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL func_020fa6f8(void);
BOOL CanUseCoPower(int player, int slot);
BOOL CanUseSuperPower(int player, int slot);
int CoPowerCost(int player, int slot);
int SuperPowerCost(int player, int slot);
s32 HwDiv32(s32 numerator, s32 denominator);
void AiProfile_RerandomiseForPlayer(int player);
void AiPlayer_PerformAction(int action);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT (STATE->players[STATE->currentPlayer])
/* AiProfile.unk_00 holds a function the AI asks before using a power: 1 for a
 * CO Power, 2 for a Super CO Power or tag power. Without one it never does. */
#define POWER_HOOK ((BOOL (*)(int))gAiProfile->unk_00)

/* Decides whether the AI player uses a power now and performs AI action 16
 * (CO Power), 17 (Super CO Power) or 18 (Super powers of both COs, with a
 * back CO). Does nothing while a power is on. Colin's CO Power is used as
 * soon as it is ready. A Super CO Power with a back CO also needs the back
 * CO's charge to be at most AiProfile.unk_06 percent of its Super cost and
 * the turn's roll at most unk_07. The CO Power is used when the front charge
 * is at most CO Power cost plus unk_04 percent of the gap to the Super cost,
 * the roll is under unk_05, and the back CO has no Super power ready. */
void AiPlayer_TryUsePower(void) {
  int coCost;
  int threshold;
  u32 hasPartner; /* needed to match: a u32 keeps the compare with TRUE */
  int powerCost;

  if (func_020fa6f8() == TRUE) {
    return;
  }
  if (CURRENT.front.powerState != 0) {
    return;
  }
  hasPartner = CURRENT.back.id != 0;
  if (hasPartner == TRUE && CanUseSuperPower(STATE->currentPlayer, 0) &&
      CanUseSuperPower(STATE->currentPlayer, 1)) {
    if (POWER_HOOK == NULL) {
      return;
    }
    if (POWER_HOOK(2) != TRUE) {
      return;
    }
    AiProfile_RerandomiseForPlayer(STATE->currentPlayer);
    AiPlayer_PerformAction(18);
    return;
  }
  if ((CURRENT.front.id & PLAYER_CO_ID_MASK) == CO_COLIN &&
      CanUseCoPower(STATE->currentPlayer, 0) == TRUE && POWER_HOOK != NULL &&
      POWER_HOOK(1) == TRUE) {
    AiProfile_RerandomiseForPlayer(STATE->currentPlayer);
    AiPlayer_PerformAction(16);
    return;
  }
  if (CanUseSuperPower(STATE->currentPlayer, 0)) {
    hasPartner = CURRENT.back.id != 0;
    if (hasPartner == TRUE) {
      powerCost = SuperPowerCost(STATE->currentPlayer, 1);
      if ((u32)HwDiv32(CURRENT.back.charge * 100, powerCost) >
          gAiProfile->unk_06) {
        return;
      }
      if (gAiTurnState->roll > gAiProfile->unk_07) {
        return;
      }
    }
    if (POWER_HOOK != NULL && POWER_HOOK(2) == TRUE) {
      AiProfile_RerandomiseForPlayer(STATE->currentPlayer);
      AiPlayer_PerformAction(17);
      return;
    }
  }
  if (!CanUseCoPower(STATE->currentPlayer, 0)) {
    return;
  }
  hasPartner = CURRENT.back.id != 0;
  if (hasPartner == TRUE && CanUseSuperPower(STATE->currentPlayer, 1)) {
    return;
  }
  coCost = CoPowerCost(STATE->currentPlayer, 0);
  powerCost = SuperPowerCost(STATE->currentPlayer, 0) - coCost;
  threshold = coCost + HwDiv32(powerCost * gAiProfile->unk_04, 100);
  if (threshold < (int)CURRENT.front.charge) {
    return;
  }
  if (gAiTurnState->roll >= gAiProfile->unk_05) {
    return;
  }
  if (POWER_HOOK == NULL) {
    return;
  }
  if (POWER_HOOK(1) != TRUE) {
    return;
  }
  AiProfile_RerandomiseForPlayer(STATE->currentPlayer);
  AiPlayer_PerformAction(16);
}
