#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL CanUseSuperPower(int player, int slot);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT (STATE->players[STATE->currentPlayer])
#define FOGGY \
  (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN)

/* Whether the AI player lets a unit of this type take the threat maps into
 * account (the callers skip the threat code when this is FALSE). Never when the
 * front CO's half of a tag power is on or the AI behaviour is STRIKE. Also not
 * for an Eagle player's units other than Infantry and Mech, while the Super CO
 * Power of each CO it has is ready, early in the turn (before AI phase 32 with
 * fog or rain, 27 without). Meaning of the phase numbers unknown. */
BOOL Ai_UnitUsesThreatMaps(int unitType) {
  u32 hasPartner; /* needed to match: a u32 gets a separate compare */

  if (CURRENT.tagPowerHalf == TAG_POWER_FRONT) {
    return FALSE;
  }
  if (CURRENT.aiBehaviour == AI_BEHAVIOUR_STRIKE) {
    return FALSE;
  }
  if ((CURRENT.front.id & PLAYER_CO_ID_MASK) == CO_EAGLE &&
      unitType != UNIT_INFANTRY && unitType != UNIT_MECH) {
    if ((FOGGY && gAiTurnState->phase < 32) ||
        (!FOGGY && gAiTurnState->phase < 27)) {
      hasPartner = CURRENT.back.id != 0;
      if (!hasPartner) {
        if (CanUseSuperPower(STATE->currentPlayer, 0)) {
          return FALSE;
        }
      } else if (CanUseSuperPower(STATE->currentPlayer, 0) &&
                 CanUseSuperPower(STATE->currentPlayer, 1)) {
        return FALSE;
      }
    }
  }
  return TRUE;
}
