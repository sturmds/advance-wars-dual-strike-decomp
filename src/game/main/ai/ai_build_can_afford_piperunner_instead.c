#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL AiBuild_WantsPiperunnerInstead(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (STATE->players[STATE->currentPlayer])

/* AiBuild_WantsPiperunnerInstead, but only when the player has the funds for
 * a Piperunner. */
BOOL AiBuild_CanAffordPiperunnerInstead(int unitType) {
  if (Unit_GetValuePerHp(STATE->currentPlayer, UNIT_PIPERUNNER) * 10 >
      (u32)PLAYER.funds) {
    return FALSE;
  }
  return AiBuild_WantsPiperunnerInstead(unitType);
}
