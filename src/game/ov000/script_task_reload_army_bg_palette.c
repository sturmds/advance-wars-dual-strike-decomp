#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022977a4.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL EventScript_IsRunning(void);
void Palette_LoadPlayerArmyBgRow(int player);
void Task_ResumeScript(Task* task);

/* Waits for the event script to stop, then reloads the current player's army
 * palette (BG) with palette slot 0 recorded, and resumes the script task. */
void ScriptTask_ReloadArmyBgPalette(Task* task) {
  if (EventScript_IsRunning() != FALSE) {
    return;
  }
  data_022977a4.unk_00 = 0;
  Palette_LoadPlayerArmyBgRow(STATE->currentPlayer);
  Task_ResumeScript(task);
}
