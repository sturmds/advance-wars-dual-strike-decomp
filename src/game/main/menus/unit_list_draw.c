#include "types.h"
#include "game/menu_state.h"
#include "game/unit_list_task.h"

int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);
void Sound_PlaySe(u32 soundId);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void UnitList_DrawRows(int scroll);

extern MenuState gMenuState;
extern u8 data_027c0028;
extern u8 data_027c0040;
extern u8 data_027c0058[];
extern u8 data_027c0060[];

/* Per-frame update of the unit list menu. Moves scroll one half row toward
 * topRow every other frame, redrawing the rows and playing a sound on odd
 * steps. Then draws two header sprites, a bar under the column heading
 * of the sort mode, and cycles one colour (palette buffer offset 0x298)
 * through the 16 colours at the start of gMenuState. */
void UnitList_Draw(UnitListTask* task) {
  if (task->scrollDelay == 0) {
    if (task->scroll != task->topRow * 2) {
      if (task->scroll > task->topRow * 2) {
        task->scroll--;
      } else {
        task->scroll++;
      }
      task->scrollDelay = 1;
      UnitList_DrawRows(task->scroll);
      if (task->scroll & 1) {
        Sound_PlaySe(0x10002);
      }
    }
  } else {
    task->scrollDelay--;
  }
  Sprite_QueueAt(0, 8, 8, &data_027c0060, 0x6194, 0);
  Sprite_QueueAt(0, 208, 8, &data_027c0040, 0x519e, 0);
  switch (task->sortMode) {
    case 0:
      Sprite_QueueAt(0, 56, 58, &data_027c0058, 0x459c, 0);
      Sprite_QueueAt(0, 72, 58, &data_027c0058, 0x459c, 0);
      Sprite_QueueAt(0, 88, 58, &data_027c0058, 0x459c, 0);
      Sprite_QueueAt(0, 104, 58, &data_027c0058, 0x459c, 0);
      Sprite_QueueAt(0, 120, 58, &data_027c0028, 0x459c, 0);
      break;
    case 1:
      Sprite_QueueAt(0, 136, 58, &data_027c0058, 0x459c, 0);
      break;
    case 2:
      Sprite_QueueAt(0, 160, 58, &data_027c0058, 0x459c, 0);
      Sprite_QueueAt(0, 176, 58, &data_027c0058, 0x459c, 0);
      Sprite_QueueAt(0, 192, 58, &data_027c0028, 0x459c, 0);
      break;
    case 3:
      Sprite_QueueAt(0, 208, 58, &data_027c0058, 0x459c, 0);
      Sprite_QueueAt(0, 224, 58, &data_027c0058, 0x459c, 0);
      break;
  }
  Palette_Load((u16*)&gMenuState + ((MainLoop_GetFrameCount() >> 2) & 15),
               0x298, 2);
}
