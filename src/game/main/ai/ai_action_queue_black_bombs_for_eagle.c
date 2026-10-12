#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL CanUseSuperPower(int player, int slot);
void AiBlackBomb_Act(void); /* run on each queued Black Bomb */

/* Same as AiAction_QueueBlackBombs, but only when the front CO is Eagle
 * and can use the Super CO Power; otherwise the queue stays empty. The
 * table runs this step earlier in the turn than AiAction_QueueBlackBombs. */
void AiAction_QueueBlackBombsForEagle(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  if ((STATE->players[STATE->currentPlayer].front.id & PLAYER_CO_ID_MASK) ==
          CO_EAGLE &&
      CanUseSuperPower(STATE->currentPlayer, 0)) {
    for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
         i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
      unit = &STATE->units[i];
      if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
          unit->type == UNIT_BLACK_BOMB) {
        *gAiQueueCursor++ = i;
      }
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiBlackBomb_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
