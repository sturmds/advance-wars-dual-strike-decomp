#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "sdk/os.h"

void Gfx_RequestLayerCopy(int bg);
void Data_Unpack(const void* src, void* dest);
u8* File_Load(const char* path);
void Mem_Free(void* res);
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

/* Loads wallpaper `index` (0 if out of range) onto layer 3 of the current
 * screen: tiles, tile map, and its palette into extended palette slot 3
 * (offset 0x6000) of whichever engine the current screen is. */
void Wallpaper_LoadOnLayer3(u32 index) {
  u8* res;

  if (index >= WALLPAPER_COUNT) {
    index = 0;
  }
  FS_ChangeDir(data_02159648);
  res = File_Load(data_021595cc[index]);
  Data_Unpack(
      res + data_0229c684[index],
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
  Data_Unpack(res + data_021593fc[index], data_027c0280->layer3Map);
  DC_FlushRange(res + data_021593bc[index], 0x200);
  if (data_027c027c == data_027c0288.unk_00) {
    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(res + data_021593bc[index], 0x6000, 0x200);
    GX_EndLoadBGExtPltt();
  } else {
    func_0210c610();
    GXS_LoadBGExtPltt(res + data_021593bc[index], 0x6000, 0x200);
    GXS_EndLoadBGExtPltt();
  }
  Gfx_RequestLayerCopy(3);
  Mem_Free(res);
}
