#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

void Task_Destroy(Task* task);
void PowerHighlight_LoadEffect(int player);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define FRONT(player) (STATE->players[player].front)

/* Update of the task that highlights units for a power. Ends the task when
 * the current player's front CO block has no unit filter. The block
 * is picked by unk_12, as in func_020e1acc, not by powerState. */
void PowerHighlightTask_Update(Task* task) {
  if (gCoTable[FRONT(STATE->currentPlayer).id & PLAYER_CO_ID_MASK]
          .blocks[FRONT(STATE->currentPlayer).unk_12]
          .affectsUnit == NULL) {
    Task_Destroy(task);
    return;
  }
  PowerHighlight_LoadEffect(STATE->currentPlayer);
  task->unk_68 = 1;
  task->unk_64 = 0;
}
