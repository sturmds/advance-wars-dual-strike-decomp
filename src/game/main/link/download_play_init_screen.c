#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Gfx_InitBothScreens(void);
void Wallpaper_LoadBoth(void);
void Screen_SetCurrentBoth(void* state);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
void LargeFont_Load(void);
void* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Mem_Free(void* file);
void Gfx_RequestLayerCopy(int layer);
void ModeFont_Load(int a, int b);
void ModeFont_ShowText(const void* text, int a, int b, int c, int d);
const void* Text_Get(u32 textId);
void SpriteTask_Create(int a, int b, int c, int d);
void DownloadPlay_Start(void);

extern const char data_0216a584[]; /* "/commo/res_linkcommon_lang" */
extern const char data_0216a5a0[]; /* "/commo/res_linklogin" */

/* Sets up the DS Download Play host screen (the child program is
 * /boot/mb_combat_sgn.srl): the link menu backgrounds from
 * /commo/res_linkcommon_lang and /commo/res_linklogin, layer 2 moved to
 * palette 1, the title text, then starts the Download Play session. Task
 * script command in data_0216a4ec. */
void DownloadPlay_InitScreen(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u8* file;
  int i;

  Gfx_InitBothScreens();
  Wallpaper_LoadBoth();
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  LargeFont_Load();

  file = File_Load(File_LocalizePath(data_0216a584));
  Data_Unpack(
      file + 0x2d8,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Mem_Free(file);

  file = File_Load(data_0216a5a0);
  Data_Unpack(file + 0x5f4, data_027c0280->layer2Map);
  Palette_Load(file + 0x6cc, 0x20, 0x20);
  Mem_Free(file);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer2Map[i] += 0x1000; /* palette 1 */
  }
  Gfx_RequestLayerCopy(2);

  ModeFont_Load(0, 0);
  ModeFont_ShowText(Text_Get(0x7d00001a), 0, 0, 0, 0);
  SpriteTask_Create(0xe8, 8, 0x3f0, 15);
  DownloadPlay_Start();
}
