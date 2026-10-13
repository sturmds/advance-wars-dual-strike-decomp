#include "types.h"
#include "game/task.h"
#include "game/play_records.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "sdk/os.h"
#include "game/wallpaper_task.h"

void Gfx_RequestLayerCopy(int bg);
void Palette_Load(const void* src, int offset, u32 size);
void Data_Unpack(const void* src, void* dest);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
u8* File_Load(const char* path);
void Mem_Free(void* res);
void Mem_Copy32(const void* src, void* dst, u32 size);
void FS_ChangeDir(const char* dir);
void GX_BeginLoadBGExtPltt(
    void); /* guess: begins a background extended palette load */
void GX_LoadBGExtPltt(const void* src, u32 offset, u32 size);
void GX_EndLoadBGExtPltt(void); /* guess: ends the load */
int func_0210c610(void);        /* same three for the sub screen */
void GXS_LoadBGExtPltt(const void* src, u32 offset, u32 size);
void GXS_EndLoadBGExtPltt(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_02159668[]; /* "/ohashi/res_wall_base" */
extern char data_02159648[]; /* "/ohashi" */
extern const char*
    data_021595cc[];        /* "res_wall_default", "res_wall_cathy", ... */
extern u16 data_021593fc[]; /* per wallpaper: offset of its tile map */
extern u16 data_021593bc[]; /* per wallpaper: offset of its palette */
extern const TaskScriptCmd gWallpaperTaskScript[];

#define WALLPAPER_COUNT 31

/* Graphics state of the first screen, data_027c0288.unk_00. */
#define FIRST_GFX ((Unk_027c0280*)data_027c0288.unk_00)

#define LAYER_CHARS(gfx, bg) ((gfx)->bgVram + ((gfx)->bgCnt[bg].charBase << 14))

/* Loads the wallpaper base onto layer 2 of both screens, then the
 * wallpaper picked in gPlayRecords.wallpaper onto layer 3 of the first screen
 * and copies it to the second. Starts the gWallpaperTaskScript task on both. */
void Wallpaper_LoadSelected(void) {
  u8* res;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16* map;
  int i;
  u32 index;

  index = gPlayRecords.wallpaper;
  if (index >= WALLPAPER_COUNT) {
    index = 0;
    gPlayRecords.wallpaper = index;
  }

  res = File_Load(data_02159668);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Data_Unpack(res + 0xd70, LAYER_CHARS(data_027c0280, data_027c0280->layer2Bg));
  Data_Unpack(res + 0x169c, data_027c0280->layer2Map);
  Palette_Load(res + 0x1a90, 0x1e0, 0x20);
  map = data_027c0280->layer2Map;
  for (i = 0; i < 0x800; i++) {
    *map += 0xf000;
    map++;
  }
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Data_Unpack(res, LAYER_CHARS(data_027c0280, data_027c0280->layer2Bg));
  Data_Unpack(res + 0x984, data_027c0280->layer2Map);
  Palette_Load(res + 0x1a90, 0x1e0, 0x20);
  map = data_027c0280->layer2Map;
  for (i = 0; i < 0x800; i++) {
    *map += 0xf000;
    map++;
  }
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(res);

  FS_ChangeDir(data_02159648);
  res = File_Load(data_021595cc[index]);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Data_Unpack(res + data_0229c684[index],
              LAYER_CHARS(data_027c0280, data_027c0280->layer3Bg));
  Data_Unpack(res + data_021593fc[index], data_027c0280->layer3Map);
  DC_FlushRange(res + data_021593bc[index], 0x200);
  GX_BeginLoadBGExtPltt();
  GX_LoadBGExtPltt(res + data_021593bc[index], 0x6000, 0x200);
  GX_EndLoadBGExtPltt();
  Gfx_RequestLayerCopy(3);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  /* charBase is used as an address here */
  Mem_Copy32((void*)GFX_LAYER(FIRST_GFX, 3)->charBase,
             LAYER_CHARS(data_027c0280, data_027c0280->layer3Bg), 0x10000);
  Mem_Copy32(GFX_LAYER(FIRST_GFX, 3)->map, data_027c0280->layer3Map, 0x1000);
  func_0210c610();
  GXS_LoadBGExtPltt(res + data_021593bc[index], 0x6000, 0x200);
  GXS_EndLoadBGExtPltt();
  Gfx_RequestLayerCopy(3);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(res);

  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Task_Create(gWallpaperTaskScript, TASK_LIST(3));
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Task_Create(gWallpaperTaskScript, TASK_LIST(3));
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
