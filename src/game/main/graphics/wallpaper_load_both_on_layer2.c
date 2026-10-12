#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "sdk/os.h"

void Gfx_RequestLayerCopy(int bg);
void Data_Unpack(const void* src, void* dest);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
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
extern char data_02159648[]; /* "/ohashi" */
extern const char*
    data_021595cc[];        /* "res_wall_default", "res_wall_cathy", ... */
extern u16 data_0229c684[]; /* per wallpaper: offset of its tiles in the file */
extern u16 data_021593fc[]; /* per wallpaper: offset of its tile map */
extern u16 data_021593bc[]; /* per wallpaper: offset of its palette */

#define WALLPAPER_COUNT 31

/* Graphics state of the first screen, data_027c0288.unk_00. */
#define FIRST_GFX ((Unk_027c0280*)data_027c0288.unk_00)

#define LAYER_CHARS(gfx, bg) ((gfx)->bgVram + ((gfx)->bgCnt[bg].charBase << 14))

/* Loads wallpaper `index` (0 if out of range) onto layer 2 of the first
 * screen, with its palette in extended palette slot 2, then copies the tiles
 * and tile map to layer 2 of the second screen. */
void Wallpaper_LoadBothOnLayer2(u32 index) {
  u8* res;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (index >= WALLPAPER_COUNT) {
    index = 0;
  }
  FS_ChangeDir(data_02159648);
  res = File_Load(data_021595cc[index]);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Data_Unpack(res + data_0229c684[index],
              LAYER_CHARS(data_027c0280, data_027c0280->layer2Bg));
  Data_Unpack(res + data_021593fc[index], data_027c0280->layer2Map);
  DC_FlushRange(res + data_021593bc[index], 0x200);
  GX_BeginLoadBGExtPltt();
  GX_LoadBGExtPltt(res + data_021593bc[index], 0x4000, 0x200);
  GX_EndLoadBGExtPltt();
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  /* charBase is used as an address here */
  Mem_Copy32((void*)GFX_LAYER(FIRST_GFX, 2)->charBase,
             LAYER_CHARS(data_027c0280, data_027c0280->layer2Bg), 0x10000);
  Mem_Copy32(FIRST_GFX->layer2Map, data_027c0280->layer2Map, 0x1000);
  func_0210c610();
  GXS_LoadBGExtPltt(res + data_021593bc[index], 0x4000, 0x200);
  GXS_EndLoadBGExtPltt();
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(res);
}
