#include "types.h"
#include "game/Unk_02080440.h"
#include "game/link_reaction_task.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);
void Task_ResumeScript(void* task);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_027c0038[];
extern u8 data_021605b8[];

/* Draws the glyphs at the task's position and scrolls them left; resumes the
 * script once the whole line has left the screen. Nothing is drawn while
 * unk_5b is set. */
void LinkReactionTask_Update(LinkReactionTask* task) {
  int i;
  int width = 0;

  if (data_027c027c->unk_48[data_027c0284]->unk_5b == 0) {
    for (i = 0; i < task->glyphCount; i++) {
      int x = task->x + width;

      if (x >= -7 && x < 256) {
        Oam_PutSprite(x & 0x1ff, task->y & 0xff, &data_027c0038,
                      0x8000 | (task->glyphs[i] * 2 + 0x94));
      }
      width += data_021605b8[task->glyphs[i]];
    }
  }
  task->x -= 2;
  if (task->x < -width) {
    Task_ResumeScript(task);
  }
}
