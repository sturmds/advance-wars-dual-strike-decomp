#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
u8* File_Load(const char* path);
void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* block);
void SpriteStrip_LoadMapName(int mapId, int arg);
void TitleCall_SetBg3Zoom(int scale, int angle);
void Screen_SetBothNibbles(int value);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u32 data_027c0284;
extern const u8
    data_0215bc40[]; /* main screen layer config for Screen_InitLayers */
extern const u8 data_0215bc90[];   /* sub screen layer config */
extern const char data_0215bee8[]; /* "/ohashi/res_titlecall_wall1" */
extern const char data_0215bf04[]; /* "/ohashi/res_titlecall_wall2" */
extern const char data_0215bf20[]; /* "/ohashi/res_titlecall" */

#define MAIN_MAP (data_027c0288.unk_00)
#define SUB_MAP (data_027c0288.unk_04)
#define MAIN_GFX ((Unk_027c0280*)data_027c0288.unk_00)

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

#define CHAR_BASE(layerBg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* BG mode 1, all BGs and OBJ on, the whole screen darkened to black for the
 * fade-in. Layers 1 and 2 start at x scroll 136. */
#define SET_UP_LAYERS(config)                                                \
  Screen_SetBgMode(1);                                                       \
  Screen_InitLayers(config);                                                 \
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 1;                \
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 2;                \
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 3;                \
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 0;                \
  data_027c0280->dispCnt |= 0x100;                                           \
  data_027c0280->dispCnt |= 0x200;                                           \
  data_027c0280->dispCnt |= 0x400;                                           \
  data_027c0280->dispCnt |= 0x800;                                           \
  data_027c0280->dispCnt |= 0x1000;                                          \
  data_027c0280->blendMode = BLEND_MODE_DARKEN;                              \
  data_027c0280->blendEva = 0;                                               \
  data_027c0280->blendEvb = 0;                                               \
  data_027c0280->blendEvy = 16;                                              \
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;                    \
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;                    \
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;                    \
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;                    \
  data_027c0280->blendTopObj = 1;                                            \
  data_027c0280->dispCnt |= 0x40;                                            \
  data_027c0280->unk_dd = 1;                                                 \
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;                     \
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;                     \
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;                     \
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 136;                   \
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;                     \
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 136;                   \
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;                     \
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;                     \
  /* BG3 is a rotation/scaling BG in mode 1: bit 13 makes it wrap around. */ \
  data_027c0280->bgCnt[data_027c0280->layer3Bg].extPltt = 1;                 \
  data_027c0280->dispCnt &= ~0x2000;                                         \
  data_027c0280->dispCnt &= ~0x4000;                                         \
  data_027c0280->dispCnt &= ~0x8000

/* Frame counter of the later steps (Screens_ZoomInFadeUpdate). */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* First step of the title call task script (data_0215bc10), shown before a
 * map starts. Both screens start darkened to black. res_titlecall_wall1 goes
 * to sub screen layers 1 and 2, res_titlecall_wall2 to the same main screen
 * layers, and res_titlecall to main screen layer 0, layer 3 (copied to the
 * sub screen) and the sprites. Then it draws the map name (SpriteStrip_LoadMapName)
 * and sets the starting BG3 zoom. */
void TitleCall_SetUpScreens(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u8* file;
  int i;

  SWITCH_TO(MAIN_MAP);
  SET_UP_LAYERS(data_0215bc40);
  SWITCH_BACK();

  SWITCH_TO(SUB_MAP);
  SET_UP_LAYERS(data_0215bc90);
  SWITCH_BACK();

  file = File_Load(data_0215bee8);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(SUB_MAP);
  Data_Unpack(file, CHAR_BASE(data_027c0280->layer1Bg));
  Data_Unpack(file + 0x4794, data_027c0280->layer1Map);
  Data_Unpack(file + 0x2cd0, CHAR_BASE(data_027c0280->layer2Bg));
  Data_Unpack(file + 0x4d58, data_027c0280->layer2Map + 0x400);
  /* both maps use BG palette 1 */
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer1Map[i] += 0x1000;
    data_027c0280->layer2Map[i + 0x400] += 0x1000;
  }
  Palette_Load(file + 0x50ac, 0x20, 0x100);
  Gfx_RequestLayerCopy(1);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(file);

  file = File_Load(data_0215bf04);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(MAIN_MAP);
  Data_Unpack(file, CHAR_BASE(data_027c0280->layer1Bg));
  Data_Unpack(file + 0x5b80, data_027c0280->layer1Map);
  Data_Unpack(file + 0x3e04, CHAR_BASE(data_027c0280->layer2Bg));
  Data_Unpack(file + 0x620c, data_027c0280->layer2Map + 0x400);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer1Map[i] += 0x1000;
    data_027c0280->layer2Map[i + 0x400] += 0x1000;
  }
  Palette_Load(file + 0x65f4, 0x20, 0x100);
  Gfx_RequestLayerCopy(1);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(file);

  file = File_Load(data_0215bf20);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(MAIN_MAP);
  Data_Unpack(file + 0x1818, CHAR_BASE(data_027c0280->layer0Bg));
  Data_Unpack(file + 0x3194, data_027c0280->layer0Map);
  /* BG palette 9 */
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer0Map[i] += 0x9000;
  }
  Palette_Load(file + 0x3578, 0x120, 0x60);
  Data_Unpack(file + 0x7f8, CHAR_BASE(data_027c0280->layer3Bg));
  Data_Unpack(file + 0x14cc, data_027c0280->layer3Map);
  Palette_Load(file + 0x17f8, 0, 0x20);
  Gfx_RequestLayerCopy(3);
  Data_Unpack(file, data_027c0280->objVram);
  Palette_Load(file + 0x7d8, 0x200, 0x20);
  Palette_Load(file + 0x35d8, 0x220, 0x20);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(SUB_MAP);
  Mem_Copy32(MAIN_GFX->unk_120, CHAR_BASE(data_027c0280->layer3Bg),
             BG_CHAR_BLOCK_SIZE);
  Mem_Copy32(MAIN_GFX->layer3Map, data_027c0280->layer3Map, GFX_LAYER_MAP_SIZE);
  Palette_Load(file + 0x17f8, 0, 0x20);
  Gfx_RequestLayerCopy(3);
  Data_Unpack(file + 0x3a0, data_027c0280->objVram);
  Palette_Load(file + 0x7d8, 0x200, 0x20);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(file);

  SpriteStrip_LoadMapName(data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId,
                          0x90);
  TitleCall_SetBg3Zoom(0x100, 0x80);
  Screen_SetBothNibbles(0);
  FRAME(task) = 0;
}
