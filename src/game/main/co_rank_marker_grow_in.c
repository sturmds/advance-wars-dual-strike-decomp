#include "types.h"
#include "game/Unk_02031a14.h"
#include "game/menu_state.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(Unk_02031a14* task);
void* SpriteAnimTask_Start(void* file, int x, int y, int id, int a, int b);

extern MenuState gMenuState;
extern s16 gSineTable[];

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Last step of the CO rank screen's marker task (data_0215b004, started by
 * CoRankMarker_Start). Over frames 0 to 4, draws sprite tile 0x174 through
 * affine matrix task->matrix while the eased divisor rises from 16 to 256,
 * so the sprite grows from 1/16 to full size; on frame 0 it also starts
 * sprite animation 0x41f4 at (x + 24, y + 24). Then resumes the script.
 * Same shape as func_02031a48. */
void CoRankMarker_GrowIn(Unk_02031a14* task) {
  int scale = Math_Ease(1, 16, 256, task->frame, 4);

  /* Needed to match: the divisor is evaluated once per entry, before the
     * table reads. */
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int sinValue = gSineTable[0];
    int cosValue = gSineTable[64] * 16;

    Oam_SetAffineParams(task->matrix, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                        HwDiv32(cosValue, d3));
  }
  Sprite_Queue(1, (task->x & 0x1ff) | (task->matrix << 9),
               ((task->y - 16) & 0xff) | 0x100, &data_027c0030, 0x4000, 0x174);
  if (task->frame == 0) {
    SpriteAnimTask_Start(&gMenuState, (task->x + 24) & 0x1ff,
                         (task->y + 24) & 0xff, 0x41f4, 0, 0);
  }
  if (task->frame != 4) {
    task->frame++;
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
