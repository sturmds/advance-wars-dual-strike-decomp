#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Gfx_RequestLayerCopy(int bg);
void Palette_Load(const void* src, int offset, u32 size);
void Data_Unpack(const void* src, void* dest);
u8* File_Load(const char* path);
void Mem_Free(void* res);

extern char data_02159650[]; /* "/ohashi/res_wall_single" */
extern const TaskScriptCmd data_0215937c[];

/* Loads the wallpaper file onto layer 3 of the screen in data_027c0288.unk_04:
 * tiles, tile map and one palette (bank 15), then adds 0xf000 to every map
 * entry so it uses that bank, and starts the task that runs data_0215937c. */
void Wallpaper_LoadSingle(void) {
  u8* res;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16* map;
  int i;

  res = File_Load(data_02159650);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Data_Unpack(
      res, data_027c0280->bgVram +
               (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
  Data_Unpack(res + 0x2eac, data_027c0280->layer3Map);
  Palette_Load(res + 0x36ec, 0x1e0, 0x20);
  map = data_027c0280->layer3Map;
  for (i = 0; i < GFX_LAYER_MAP_ENTRIES; i++) {
    *map += 0xf000;
    map++;
  }
  Gfx_RequestLayerCopy(3);
  Task_Create(data_0215937c, TASK_LIST(0));
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(res);
}
