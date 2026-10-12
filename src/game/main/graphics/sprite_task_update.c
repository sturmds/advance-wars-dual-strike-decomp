#include "types.h"
#include "game/sprite_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_027c0040[];

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
int WM_GetLinkLevel(void);
void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

/* Puts the task's sprite into OAM of the second screen (data_027c0288.unk_04), shifted by 4 tiles
 * times the value of WM_GetLinkLevel (an SDK function; meaning not known). */
void SpriteTask_Update(SpriteTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Oam_PutCells(task->x & 0x1ff, task->y & 0xff, data_027c0040,
               task->paletteSlot << 12, task->tile + WM_GetLinkLevel() * 4);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
