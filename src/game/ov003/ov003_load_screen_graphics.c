#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/co_graphics.h"
#include "game/ov003_gfx.h"
#include "game/menu_state.h"
#include "game/ov003_unit.h"

extern u8* data_ov003_02365b10;
static char data_ov003_02364934[] = {
#include "data/ov003/data_ov003_02364934.inc.c"
};
static char data_ov003_0236494c[] = {
#include "data/ov003/data_ov003_0236494c.inc.c"
};
static char data_ov003_02364964[] = {
#include "data/ov003/data_ov003_02364964.inc.c"
};
extern char data_ov003_02364984[];
extern char data_ov003_023649a0[];
extern char data_ov003_023649c0[];
extern char data_ov003_023649d8[];

void Screen_SetCurrentBoth(void* state);
void* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Mem_Free(void* file);
void* Mem_Alloc(u32 size);
void Wallpaper_LoadSingle(void);
int CombatCo_IdFromNumber(int value);
void Co_LoadArmyGraphicsToBuffer(int a, void* dst, int b);
void Ov003_LoadMapPanelGraphics(void);
void Ov003_LoadGraphics(void* file);
void Ov003_LoadMainScreenGraphics(void* file, void* localized);
void Ov003_LoadObjGraphics(void* file, void* localized);
void Ov003_LoadSubScreenGraphics(void* file, void* localized);
void Ov003_UploadCoPictures(void);

/* Loads the files for the screens on both screens, then a 0x3600-byte picture
 * (Co_LoadArmyGraphicsToBuffer, a CO graphics loader) for each active row. Pictures 0 and 1
 * go to gMenuState; the rest go to a buffer allocated here. */
void Ov003_LoadScreenGraphics(void) {
  void* file;
  void* localized;
  int i;
  u8* dst;
  Ov003Row* row;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Ov003_LoadMapPanelGraphics();
  file = File_Load(data_ov003_02364934);
  Ov003_LoadGraphics(file);
  Mem_Free(file);

  file = File_Load(data_ov003_0236494c);
  localized = File_Load(File_LocalizePath(data_ov003_02364964));
  Ov003_LoadMainScreenGraphics(file, localized);
  Mem_Free(localized);
  Mem_Free(file);

  file = File_Load(data_ov003_02364984);
  localized = File_Load(File_LocalizePath(data_ov003_023649a0));
  Ov003_LoadObjGraphics(file, localized);
  Mem_Free(localized);
  Mem_Free(file);

  file = File_Load(data_ov003_023649c0);
  localized = File_Load(File_LocalizePath(data_ov003_023649d8));
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Ov003_LoadSubScreenGraphics(file, localized);
  Mem_Free(localized);
  Mem_Free(file);

  Wallpaper_LoadSingle();
  data_ov003_02365b10 =
      Mem_Alloc(OV003_PICTURES_ALLOCATED * CO_LARGE_PICTURE_SIZE);
  for (i = 0; i < data_ov003_02364928->rowCount; i++) {
    row = &data_ov003_02364928->rows[i];
    if (row->active) {
      if (i < OV003_PICTURES_IN_MENU_STATE) {
        dst = (u8*)&gMenuState + i * CO_LARGE_PICTURE_SIZE;
      } else {
        dst = data_ov003_02365b10 +
              (i - OV003_PICTURES_IN_MENU_STATE) * CO_LARGE_PICTURE_SIZE;
      }
      Co_LoadArmyGraphicsToBuffer(CombatCo_IdFromNumber(row->co), dst, i + 1);
    }
  }
  Ov003_UploadCoPictures();
}
