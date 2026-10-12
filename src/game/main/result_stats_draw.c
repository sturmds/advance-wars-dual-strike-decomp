#include "types.h"
#include "game/task.h"

void ResultStats_DrawSubScreen(int offset, Task* task);
void ResultStats_DrawMainScreen(int offset, Task* task);

/* Draws both screens of the stats page, moved right by `offset` pixels. */
void ResultStats_Draw(int offset, Task* task) {
  ResultStats_DrawSubScreen(offset, task);
  ResultStats_DrawMainScreen(offset, task);
}
