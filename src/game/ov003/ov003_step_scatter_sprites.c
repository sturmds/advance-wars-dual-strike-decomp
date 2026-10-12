#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_0235dd38.h"

extern Ov003Unit data_ov003_02369538[16];

s32 HwMod32(s32 numerator, s32 denominator);
u32 Ov003_RandNext(void);
int Ov003_IsPhase0OrAbove6(void);
int func_ov003_023622b8(void);
void Ov003_CreateRowSprite(int x, int y, int row);
void Task_ResumeScript(Unk_ov003_0235dd38* task);

/* Map size in 8-pixel units (a tile is 16 of them). */
#define MAP_WIDTH_8PX (data_ov003_02364a88->width << 7 >> 3)
#define MAP_HEIGHT_8PX (data_ov003_02364a88->height << 7 >> 3)

/* Needed to match: Task.unk_4c and unk_64 are read as signed. */
#define STEP(task) ((s16)(task)->unk_4c)
#define COUNT(task) ((s16)(task)->unk_64)

/* Script command for 32 rounds of 5 steps. On the fifth step it picks a
 * random point and calls Ov003_CreateRowSprite (which creates a sprite) there with the row in
 * Task.unk_52. The point lies within 128 (8-pixel units) of a random live unit
 * of another row, clamped to the map, or anywhere on the map when there is no
 * such unit. After the 32nd round the script resumes. It does nothing while
 * Ov003_IsPhase0OrAbove6 returns 0. */
void Ov003_StepScatterSprites(Unk_ov003_0235dd38* task) {
  Ov003Unit* candidates[8];
  Ov003Unit* unit;
  int i;
  int count;
  int x;
  int y;
  s16 minX;
  s16 minY;
  s16 maxX;
  s16 maxY;

  if (Ov003_IsPhase0OrAbove6() == 0) {
    return;
  }
  if (STEP(task) == 4) {
    count = 0;
    unit = data_ov003_02369538;
    for (i = 0; i < func_ov003_023622b8(); i++, unit++) {
      if (unit->hp != 0 && !unit->unk_17_6 && unit->row != task->unk_52) {
        candidates[count++] = unit;
      }
    }
    if (count == 0) {
      minX = 0;
      minY = 0;
      maxX = (MAP_WIDTH_8PX)-1;
      maxY = (MAP_HEIGHT_8PX)-1;
    } else {
      unit = candidates[HwMod32(Ov003_RandNext(), count)];
      x = unit->x >> 3;
      minX = x - 128;
      y = unit->y >> 3;
      maxX = x + 128;
      minY = y - 128;
      maxY = y + 128;
      if (minX < 0) {
        minX = 0;
      }
      if (minY < 0) {
        minY = 0;
      }
      if (maxX >= MAP_WIDTH_8PX) {
        maxX = (MAP_WIDTH_8PX)-1;
      }
      if (maxY >= MAP_HEIGHT_8PX) {
        maxY = (MAP_HEIGHT_8PX)-1;
      }
    }
    maxX = minX + HwMod32(Ov003_RandNext(), maxX - minX);
    maxY = minY + HwMod32(Ov003_RandNext(), maxY - minY);
    Ov003_CreateRowSprite(maxX, maxY, task->unk_52);
    task->unk_64 = COUNT(task) + 1;
    task->unk_4c = 0;
    if (COUNT(task) == 32) {
      Task_ResumeScript(task);
      return;
    }
  }
  task->unk_4c = STEP(task) + 1;
}
