#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

void Gfx_ClearLayerMap(int layer);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void* File_Load(const char* path);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
void Gfx_RequestLayerCopy(int layer);
void Screen_SetLayer2Scroll(int x, int y);
void DesignMap_DrawTemplatePreview(int a, int b, int c, int d);
void Screen_SetLayer0Scroll(int x, int y);
void Ov006_ResetScreen1Effects(void);
void Ov006_LoadPropertyCountGraphics(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_ov006_0236aad8[]; /* layer config for Screen_InitLayers */
extern char gMapeditDir[];       /* "/mapedit" */
extern char gMapeditFile006[];   /* layer 3 tiles */
extern char gMapeditFile007[];   /* layer 3 map */
extern char gMapeditFile008[];   /* layer 3 palette */
extern char gMapeditFile001[];
extern char gMapeditFile009[];
extern char gMapeditFile00a[];
extern char gMapeditFile004[];
extern char gMapeditFile005[];

/* Sets up the second screen (data_027c0288.unk_04) of the Design Room: all
 * four layers on, layer 3 from /mapedit files 006 to 008, an OBJ palette
 * from 001 and sprite tiles from 009 and 004. Files 00a and 005 stay loaded
 * in the overlay state. Then runs DesignMap_DrawTemplatePreview and
 * Screen_SetLayer0Scroll for that screen (the nested switch keeps
 * data_027c027c on the first screen) and Ov006_ResetScreen1Effects. */
void Ov006_LoadMapeditGraphics(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_ClearLayerMap(2);
  Gfx_ClearLayerMap(3);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov006_0236aad8);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  FS_ChangeDir(gMapeditDir);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14),
      gMapeditFile006);
  File_UnpackTo(data_027c0280->layer3Map, gMapeditFile007);
  Palette_LoadFromFile(gMapeditFile008, 0, 0x40, 0x20);
  TileMap_AddToEntries(data_027c0280->layer3Map, 0x800, 0x2000);
  Gfx_RequestLayerCopy(3);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Gfx_RequestLayerCopy(2);
  Screen_SetLayer2Scroll(0, 0);
  Palette_LoadFromFile(gMapeditFile001, 0, 0x3e0, 0x20);
  File_UnpackTo(data_027c0280->objVram + 0x58e0, gMapeditFile009);
  data_ov006_0236d640->unk_68 = File_Load(gMapeditFile00a);
  File_UnpackTo(data_027c0280->objVram + 0x5600, gMapeditFile004);
  data_ov006_0236d640->unk_70 = File_Load(gMapeditFile005);
  /* needed to match: this block gets its own locals */
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    Unk_027c0280* savedGfx;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    DesignMap_DrawTemplatePreview(0, 0, 0x44, 7);
    Screen_SetLayer0Scroll(0x1fa, 0xd0);
    Screen_SetCurrentGfx(savedGfx);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  Ov006_ResetScreen1Effects();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Ov006_LoadPropertyCountGraphics();
}
