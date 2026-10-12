#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const TaskScriptCmd data_02169ccc[];

/* Starts a child task unless the game mode is 3. The first argument is not
 * used. */
void func_020f4c00(int unused, Task* parent) {
  if (data_027c027c->unk_48[data_027c0284]->gameMode == 3) {
    return;
  }
  Task_CreateChild(data_02169ccc, parent);
}
