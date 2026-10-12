#include "types.h"
#include "game/screen_other_map.h"
#include "game/number_sprite_task.h"
#include "game/task.h"

extern u8 data_ov000_023503a0; /* game language */
extern const char* data_ov000_022f669c[];
extern char data_02169c88[];
extern char data_02169c44[]; /* "/bmap" */
extern u8 data_02138c68[];
extern const TaskScriptCmd gNumberSpriteTaskScript[];

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_Load(const void* src, u32 offset, u32 size);

/* Loads the number font graphics at firstTile and its palette (slot palette + 16)
 * on the second screen, then starts the number task under parent. The number
 * slides in from 32 pixels above y. */
void NumberSpriteTask_Start(int style, int x, int y, int firstTile, int palette,
                            Task* parent) {
  Unk_027c0280* saved0280 = data_027c0280;
  NumberSpriteTask* task;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  FS_ChangeDir(data_02169c88);
  File_UnpackTo(data_027c0280->objVram + firstTile * 32,
                data_ov000_022f669c[data_ov000_023503a0]);
  FS_ChangeDir(data_02169c44);
  Palette_Load(data_02138c68, (u16)((palette + 16) << 5), 32);
  Screen_SetCurrentGfx(saved0280);
  task = (NumberSpriteTask*)Task_Create(gNumberSpriteTaskScript, parent);
  task->x = x;
  task->y = y;
  task->firstTile = firstTile;
  task->palette = palette;
  task->style = style;
  task->slideOffset = -32;
  task->value = 0;
}
