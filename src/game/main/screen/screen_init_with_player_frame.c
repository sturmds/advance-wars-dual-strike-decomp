#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0280* data_027c0280;
extern u8 data_02169970[];
const u32 gBgAffineMapBytes[4] = {
#include "data/main/gBgAffineMapBytes.inc.c"
};
const u32 gBgTextMapBytes[4] = {
#include "data/main/gBgTextMapBytes.inc.c"
};
static const u8 data_02134e8c[11772] = {
#include "data/main/data_02134e8c.inc.c"
};
extern u8 data_02137ca8[];
extern u8 data_02137c88[];

void MainLoop_SetFrameCallback(void (*callback)(void));
void MainLoop_TaskFrame(void);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void Screen_ResetBlend(void);
void Screen_ResetLayerScroll(void);
void Gfx_LoadPlayerWindowFrame(int player);
void Data_Unpack(const void* src, void* dst);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
void Gfx_RequestLayerCopy(int layer);
void Palette_Load(const void* src, u32 offset, u32 size);
void NumberFont_LoadLow(void);

/* Sets up the current screen: BG mode 0, the layers from data_02169970, the
 * window frame of the current player, layer 3 tiles and map (map entries get
 * palette 1, which the palette at data_02137c88 fills), then shows BG0-3 and
 * objects. Reached through the scene task scripts around 0x02169654. */
void Screen_InitWithPlayerFrame(void) {
  u16 player = data_027c027c->unk_48[data_027c0284]->currentPlayer;

  MainLoop_SetFrameCallback(MainLoop_TaskFrame);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_02169970);
  Screen_ResetBlend();
  Screen_ResetLayerScroll();
  Gfx_LoadPlayerWindowFrame(player);
  Data_Unpack(data_02134e8c, data_027c0280->unk_120);
  Data_Unpack(data_02137ca8, data_027c0280->layer3Map);
  TileMap_AddToEntries(data_027c0280->layer3Map, 0x800, 0x1000);
  Gfx_RequestLayerCopy(3);
  Palette_Load(data_02137c88, 32, 32);
  NumberFont_LoadLow();
  GFX_DISPCNT(data_027c0280)->bg0 = 1;
  GFX_DISPCNT(data_027c0280)->bg1 = 1;
  GFX_DISPCNT(data_027c0280)->bg2 = 1;
  GFX_DISPCNT(data_027c0280)->bg3 = 1;
  GFX_DISPCNT(data_027c0280)->obj = 1;
}
