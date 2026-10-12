#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void func_020d331c(void);
BOOL Map_IsCoSelectPhase(void);
void func_020e888c(void);
void Game_LoadSavesAndStartModeTask(int player, int coSelectPhase);

/* Takes a task like the one func_020ca428 creates, whose unk_64 holds the
 * Map_IsCoSelectPhase value from the time of creation. */
void Game_LeaveToModeTask(Task* task) {
  func_020d331c();
  if (Map_IsCoSelectPhase()) {
    func_020e888c();
  }
  Game_LoadSavesAndStartModeTask(
      data_027c027c->unk_48[data_027c0284]->currentPlayer, task->unk_64);
}
