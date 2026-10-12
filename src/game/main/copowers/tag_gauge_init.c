#include "types.h"
#include "game/tag_gauge_task.h"
#include "game/Unk_02080440.h"

u8 Co_GetTagFirepower(int co, int partner);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define PLAYER (&GAME->players[GAME->currentPlayer])

/* First step of the tag firepower gauge script: clears the counters and sets
 * the target to the tag firepower of the current player's two COs. */
void TagGauge_Init(TagGaugeTask* task) {
  task->frame = 0;
  task->value = 0;
  task->popInDone = 0;
  task->target = Co_GetTagFirepower(PLAYER->front.id & PLAYER_CO_ID_MASK,
                                    PLAYER->back.id & PLAYER_CO_ID_MASK);
}
