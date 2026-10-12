#include "types.h"
#include "game/task.h"

void TileEffectTask_Create(int x, int y, int effect, Task* parent);

void func_020eeddc(int x, int y, Task* parent) {
  TileEffectTask_Create(x, y, 0, parent);
}
