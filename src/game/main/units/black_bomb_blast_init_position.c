#include "types.h"
#include "game/Unk_02080440.h"
#include "game/black_bomb_blast_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Blast task step: finds the blast centre in map pixels (middle of the
 * tile) and notes the current vertical scroll. */
void BlackBombBlast_InitPosition(BlackBombBlastTask* task) {
  task->pixelX = task->tileX * 16 + 8;
  task->pixelY = task->tileY * 16 + 8;
  task->scrollY = STATE->unk_1c4->scrollY;
}
