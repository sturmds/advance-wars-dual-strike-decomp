#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/ai_profile.h"
#include "game/co_data.h"
#include "game/math.h"
#include "game/ai/ai_turn_setup.h"
#include "game/ai/ai_perform_action.h"
#include "game/player.h"
#include "game/ai/ai_power.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define CURRENT (MAP_STATE->players[MAP_STATE->currentPlayer])
/* AiProfile.unk_00 holds a function the AI asks before using a power: 1 for a
 * CO Power, 2 for a Super CO Power or tag power. Without one it never does. */
#define POWER_HOOK ((BOOL (*)(int))gAiProfile->unk_00)
/* Needed to match: the original reads data_027c0284 and then data_027c027c
 * again instead of reusing the state pointer it already holds. */
#define RELOAD_STATE(dst)                                             \
  {                                                                   \
    int index_ = *(volatile int*)&data_027c0284;                      \
    dst = (*(Unk_02080440* volatile*)&data_027c027c)->unk_48[index_]; \
  }
/* a rank per CO id, used to break ties */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

BOOL func_020fa6f8(void);
BOOL CanUseCoPower(int player, int slot);
BOOL CanUseSuperPower(int player, int slot);
int CoPowerCost(int player, int slot);
int SuperPowerCost(int player, int slot);

/* AiTurn_Update state 5, run after the unit actions; switches on
 * Player.tagPowerHalf (guess: the AI's CO power step). With no tag power
 * on, AiCo_FrontCloserToSuper decides whether to perform action 15 and the
 * turn moves on to state 6. While the front half is on, action 15 runs again and the
 * state stays 5. */
void AiTurn_RunPowerStep(void) {
  switch (MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf) {
    case 0:
      if (AiCo_FrontCloserToSuper() == TRUE) {
        AiPlayer_PerformAction(15);
      }
      gAiTurnState->state = 6;
      break;
    case 1:
      AiPlayer_PerformAction(15);
      break;
    case 2:
      gAiTurnState->state = 6;
      break;
    default:
      gAiTurnState->state = 6;
      break;
  }
}

/* Rank of a CO for the AI's tag choice: AiCo_FrontCloserToSuper compares the
 * front and back CO's ranks when their meters do not decide. Most COs rank 2. Kanbei
 * ranks 0, and Hachi 6 and Colin 7, only while gAiTurnState->factoryDomainsOwn
 * is nonzero; Sonja ranks 3 in Fog of War or rain. */
int AiCo_TagRank(int coId) {
  /* needed to match: one return per case, in this order */
  switch (coId) {
    case CO_NELL:
      return 2;
    case CO_ANDY:
      return 2;
    case CO_MAX:
      return 1;
    case CO_SAMI:
      return 4;
    case CO_HACHI:
      if (gAiTurnState->factoryDomainsOwn != 0) {
        return 6;
      }
      return 2;
    case CO_JAKE:
      return 2;
    case CO_RACHEL:
      return 2;
    case CO_OLAF:
      return 2;
    case CO_GRIT:
      return 5;
    case CO_COLIN:
      if (gAiTurnState->factoryDomainsOwn != 0) {
        return 7;
      }
      return 2;
    case CO_SASHA:
      return 2;
    case CO_KANBEI:
      if (gAiTurnState->factoryDomainsOwn != 0) {
        return 0;
      }
      return 2;
    case CO_SONJA:
      if (MAP_STATE->unk_1c4->fog != 0 ||
          MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
        return 3;
      }
      return 2;
    case CO_SENSEI:
      return 2;
    case CO_GRIMM:
      return 2;
    case CO_EAGLE:
      return 2;
    case CO_DRAKE:
      return 2;
    case CO_JESS:
      return 2;
    case CO_JAVIER:
      return 2;
    case CO_LASH:
      return 2;
    case CO_HAWKE:
      return 2;
    case CO_JUGGER:
      return 2;
    case CO_KOAL:
      return 2;
    case CO_KINDLE:
      return 2;
    case CO_VON_BOLT:
      return 2;
    case CO_NONE:
      return 2;
    default:
      return 2;
  }
}

/* Run by AiTurn_RunPowerStep while no tag power is on; TRUE makes it perform
 * AI action 15. Needs CO effects on, no power on and a back CO. Compares how
 * far each CO's meter is from its Super CO Power cost: TRUE when the front CO
 * is closer. When both are equally far, or both meters are empty,
 * AiCo_TagRank ranks the two COs instead. */
BOOL AiCo_FrontCloserToSuper(void) {
  int checkedPlayer; /* the current player, read during the checks */
  u32 hasPartner;
  Unk_02080440_48* state;
  int player; /* the current player again */
  Player* players;
  u32 frontCharge;
  int current;

  /* Needed to match: the first index read goes through frontCharge, which
     * gets its real value later (7 differing instructions without it, 4 with) */
  frontCharge = *(volatile int*)&data_027c0284;
  state = data_027c027c->unk_48[frontCharge];
  if (state->coEffectsOn == 0) {
    return FALSE;
  }
  checkedPlayer = state->currentPlayer;
  if (state->players[checkedPlayer].front.powerState != 0) {
    return FALSE;
  }
  hasPartner = state->players[checkedPlayer].back.id != 0;
  if (hasPartner == FALSE) {
    return FALSE;
  }
  RELOAD_STATE(state);
  player = state->currentPlayer;
  /* Needed to match (found by tools/permuter.py): this empty test emits
     * no code, but naming players[player] here, before players is set,
     * gives the players base and the index product the original's registers */
  if (players[player].back.charge || players[player].back.charge) {
  }
  frontCharge = state->players[player].front.charge;
  players = state->players;
  /* needed to match: the byte-offset form keeps the compiler from reusing
     * the index product of players[player] */
  if (SuperPowerCost(player, 0) - frontCharge ==
          SuperPowerCost(checkedPlayer, 1) -
              ((Player*)((u8*)state->players + player * sizeof(Player)))
                  ->back.charge ||
      (frontCharge == 0 && players[player].back.charge == 0)) {
    if (AiCo_TagRank(MAP_STATE->players[MAP_STATE->currentPlayer].front.id &
                     PLAYER_CO_ID_MASK) <
        AiCo_TagRank(players[player].back.id & PLAYER_CO_ID_MASK)) {
      return TRUE;
    }
  } else {
    RELOAD_STATE(state);
    current = state->currentPlayer;
    if (SuperPowerCost(current, 0) -
            state->players[state->currentPlayer].front.charge <
        SuperPowerCost(player, 1) - state->players[current].back.charge) {
      return TRUE;
    }
  }
  return FALSE;
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
  if (hasPartner == TRUE && CanUseSuperPower(MAP_STATE->currentPlayer, 0) &&
      CanUseSuperPower(MAP_STATE->currentPlayer, 1)) {
    if (POWER_HOOK == NULL) {
      return;
    }
    if (POWER_HOOK(2) != TRUE) {
      return;
    }
    AiProfile_RerandomiseForPlayer(MAP_STATE->currentPlayer);
    AiPlayer_PerformAction(18);
    return;
  }
  if ((CURRENT.front.id & PLAYER_CO_ID_MASK) == CO_COLIN &&
      CanUseCoPower(MAP_STATE->currentPlayer, 0) == TRUE &&
      POWER_HOOK != NULL && POWER_HOOK(1) == TRUE) {
    AiProfile_RerandomiseForPlayer(MAP_STATE->currentPlayer);
    AiPlayer_PerformAction(16);
    return;
  }
  if (CanUseSuperPower(MAP_STATE->currentPlayer, 0)) {
    hasPartner = CURRENT.back.id != 0;
    if (hasPartner == TRUE) {
      powerCost = SuperPowerCost(MAP_STATE->currentPlayer, 1);
      if ((u32)HwDiv32(CURRENT.back.charge * 100, powerCost) >
          gAiProfile->unk_06) {
        return;
      }
      if (gAiTurnState->roll > gAiProfile->unk_07) {
        return;
      }
    }
    if (POWER_HOOK != NULL && POWER_HOOK(2) == TRUE) {
      AiProfile_RerandomiseForPlayer(MAP_STATE->currentPlayer);
      AiPlayer_PerformAction(17);
      return;
    }
  }
  if (!CanUseCoPower(MAP_STATE->currentPlayer, 0)) {
    return;
  }
  hasPartner = CURRENT.back.id != 0;
  if (hasPartner == TRUE && CanUseSuperPower(MAP_STATE->currentPlayer, 1)) {
    return;
  }
  coCost = CoPowerCost(MAP_STATE->currentPlayer, 0);
  powerCost = SuperPowerCost(MAP_STATE->currentPlayer, 0) - coCost;
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
  AiProfile_RerandomiseForPlayer(MAP_STATE->currentPlayer);
  AiPlayer_PerformAction(16);
}

int AiTurn_IsPhase3(void) { return (gAiTurnState->phase == 3) ? 1 : 0; }

/* TRUE when the AI phase is 32 with fog of war or rain on the map, or 27
 * without. Called by func_02099338. */
BOOL func_02099528(void) {
  Unk_02080440_1c4* rules = MAP_STATE->unk_1c4;

  if (rules->fog != 0 || rules->weather == WEATHER_RAIN) {
    if (gAiTurnState->phase == 32) {
      return TRUE;
    }
  } else if (gAiTurnState->phase == 27) {
    return TRUE;
  }
  return FALSE;
}
