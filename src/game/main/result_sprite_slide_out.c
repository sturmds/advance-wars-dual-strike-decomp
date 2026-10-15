#include "types.h"
#include "game/Unk_022a7a9c.h"
#include "game/result_sprite_task.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"
#include "game/math.h"

void ResultPoint_RestartScript(int value);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(ResultSpriteTask* task);
void ResultPath_DrawAllOffset(int lead, int dx, int dy);

#define SLIDE_FRAMES 16

/* Script step: restarts the point panel's script (which scrolls the panel
 * left, ResultPoint_ScrollAllUpdate), and over SLIDE_FRAMES frames moves the
 * path sprites and the data_027c0060 sprite 120 pixels to the right while
 * layer 1 scrolls back. */
void ResultSprite_SlideOut(ResultSpriteTask* task) {
  int offset;

  if (task->frame == 0) {
    ResultPoint_RestartScript(SLIDE_FRAMES);
  }
  task->frame++;
  offset = Math_Ease(1, 0, 120, task->frame, SLIDE_FRAMES);
  ResultPath_DrawAllOffset(195 - (5 - data_022a7a9c.rank) * 35, offset, 0);
  Sprite_QueueAt(6, (200 + offset) & 0x1ff, 88, &data_027c0060, 0x1000, 0x40a);
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 88 - offset;
  if (task->frame == SLIDE_FRAMES) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
