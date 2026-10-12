#include "types.h"
#include "game/Unk_02080440.h"

void MapCursor_StepPastViewEdge(void);
void MapCursor_MoveToTouch(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs MapCursor_StepPastViewEdge and MapCursor_MoveToTouch unless the view is still scrolling
 * (any of the four direction bits in scrollFlags; func_02080440 makes the
 * same test before AiTurn_ShowAction). MapCursor_UpdateFree is the same
 * with the free-cursor key and touch handlers. */
void func_020c40d0(void) {
  if ((STATE->unk_1c4->scrollFlags & 0xf) != 0) {
    return;
  }
  MapCursor_StepPastViewEdge();
  MapCursor_MoveToTouch();
}
