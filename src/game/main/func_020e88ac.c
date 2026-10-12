#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void func_02050664(Task* parent);

/* Creates the task of func_02050664 as a child of `parent`, in Campaign mode only. */
void func_020e88ac(Task* parent) {
  if (data_027c027c->unk_48[data_027c0284]->gameMode != GAME_MODE_CAMPAIGN) {
    return;
  }
  func_02050664(parent);
}
