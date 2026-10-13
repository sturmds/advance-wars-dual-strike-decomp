#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/battle_save.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern u32 data_027c0284;
extern Unk_027c0288 data_027c0288;

/* The two screen contexts in DTCM; data_027c0288 points to them. */
extern u8 gMainScreenContext[];
extern u8 gSubScreenContext[];

extern u8 data_021575c8[]; /* main screen layer config */
extern u8 data_02157618[]; /* sub screen layer config */

/* The two map states. The main screen lists data_02183b08 first, the sub
 * screen data_02189cd8. */
extern Unk_02080440_48 data_02183b08;
extern Unk_02080440_48 data_02189cd8;

/* Buffers of the main screen context. */
extern struct OamBuffer data_021704a8;
extern u8 data_02177b08[];
extern u8 data_0216fca8[];
extern struct FontState data_0216f5f4[2];
extern u8 data_0216e614[];
extern struct IconBank data_0216ef94;
extern u8 data_02171108[];
extern u8 data_0216e76c[];
extern u8 data_0216e5b8[];
extern struct Task data_02173788;
extern struct Task* data_0216e96c[];
extern struct Task* data_0216e5d4[];
extern struct ScriptSlot data_02172108;
extern u16 data_0216e5b0[2];
extern s16 data_0216e5b4[2];
extern Unk_02080440_50 data_0216e6a4;
extern u8 data_0216e5ac[];
extern Unk_02080440_5c data_0216e5c0[2];
extern struct WeatherFx data_0216ebf4;

/* Buffers of the sub screen context. Four of them are the second element of
 * an array of the main screen's (data_0216e5b0, data_0216e5b4, data_0216e5c0,
 * data_0216f5f4); the original names them by address. */
extern struct OamBuffer data_02170ad8;
extern u8 data_0217db08[];
extern u8 data_021700a8[];
extern struct FontState data_0216f94e;
extern u8 data_0216e65c[];
extern struct IconBank data_0216f2c4;
extern u8 data_02171908[];
extern u8 data_0216e86c[];
extern u8 data_0216e5bc[];
extern struct Task data_02175948;
extern struct Task* data_0216eab0[];
extern struct Task* data_0216e5f4[];
extern struct ScriptSlot data_02172c48;
extern u16 data_0216e5b2;
extern s16 data_0216e5b6;
extern Unk_02080440_50 data_0216e708;
extern u8 data_0216e5ae[];
extern Unk_02080440_5c data_0216e5c9;
extern struct WeatherFx data_0216edc4;

void func_0200a630(void);
void Screen_SetCurrentBoth(void* screen);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);

/* Hardware addresses (GBATEK memory map); DB_ is the sub engine. */
#define HW_OAM ((u8*)0x07000000)
#define HW_BG_VRAM ((u8*)0x06000000)
#define HW_OBJ_VRAM ((u8*)0x06400000)
#define HW_BG_PLTT ((u8*)0x05000000)
#define HW_OBJ_PLTT ((u8*)0x05000200)
#define HW_DB_OAM ((u8*)0x07000400)
#define HW_DB_BG_VRAM ((u8*)0x06200000)
#define HW_DB_OBJ_VRAM ((u8*)0x06600000)
#define HW_DB_BG_PLTT ((u8*)0x05000400)
#define HW_DB_OBJ_PLTT ((u8*)0x05000600)

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Called once from main. Points data_027c0288 at the two screen contexts
 * and fills each one: its layers, its VRAM, OAM and palette addresses
 * (main engine first, then sub), and its buffers in main RAM. Each context
 * also gets both map states, in opposite order, and the current map state's
 * pointers to its own storage. */
void Screens_InitContexts(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  func_0200a630();
  data_027c0288.unk_00 = gMainScreenContext;
  data_027c0288.unk_04 = gSubScreenContext;
  Screen_SetCurrentBoth(gMainScreenContext);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_021575c8);
  data_027c0280->oamDest = HW_OAM;
  data_027c0280->bgVram = HW_BG_VRAM;
  data_027c0280->objVram = HW_OBJ_VRAM;
  data_027c0280->paletteCopy = HW_BG_PLTT;
  data_027c0280->objPaletteVram = HW_OBJ_PLTT;
  data_027c0280->oamBuffer = &data_021704a8;
  data_027c0280->unk_04 = data_02177b08;
  data_027c0280->paletteBuffer = data_0216fca8;
  data_027c0280->paletteUploadQueued = 0;
  data_027c0280->layerCopyMask = 0;
  data_027c0280->fontState = data_0216f5f4;
  data_027c0280->textPrinter = data_0216e614;
  data_027c0280->iconBanks = &data_0216ef94;
  data_027c027c->unk_20 = data_02171108;
  data_027c027c->unk_24 = data_0216e76c;
  data_027c027c->unk_28 = data_0216e5b8;
  data_027c027c->taskPool = &data_02173788;
  data_027c027c->taskFreeStack = data_0216e96c;
  data_027c027c->taskLists = data_0216e5d4;
  data_027c027c->scriptSlots = &data_02172108;
  data_027c027c->unk_40 = data_0216e5b0;
  data_027c027c->scriptSlotCurrent = data_0216e5b4;
  data_027c027c->unk_48[0] = &data_02183b08;
  data_027c027c->unk_48[1] = &data_02189cd8;
  data_027c027c->unk_50[0] = &data_0216e6a4;
  data_027c027c->unk_50[1] = &data_0216e708;
  data_027c027c->unk_58 = data_0216e5ac;
  data_027c027c->unk_5c[0] = data_0216e5c0;
  data_027c027c->unk_5c[1] = &data_0216e5c9;
  data_027c027c->weatherFx = &data_0216ebf4;
  STATE->unk_1c4 = (Unk_02080440_1c4*)&STATE->unk_184;
  STATE->mapTiles = (MapTiles*)STATE->mapTilesBuf;
  STATE->units = STATE_UNIT_SLOTS(STATE);
  STATE->players = STATE->playerSlots;
  STATE->tileCodeTerrain = STATE->tileCodeTerrainBuf;
  STATE->unk_6088 = STATE->unk_6088Buf;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_02157618);
  data_027c0280->oamDest = HW_DB_OAM;
  data_027c0280->bgVram = HW_DB_BG_VRAM;
  data_027c0280->objVram = HW_DB_OBJ_VRAM;
  data_027c0280->paletteCopy = HW_DB_BG_PLTT;
  data_027c0280->objPaletteVram = HW_DB_OBJ_PLTT;
  data_027c0280->oamBuffer = &data_02170ad8;
  data_027c0280->unk_04 = data_0217db08;
  data_027c0280->paletteBuffer = data_021700a8;
  data_027c0280->paletteUploadQueued = 0;
  data_027c0280->layerCopyMask = 0;
  data_027c0280->fontState = &data_0216f94e;
  data_027c0280->textPrinter = data_0216e65c;
  data_027c0280->iconBanks = &data_0216f2c4;
  data_027c027c->unk_20 = data_02171908;
  data_027c027c->unk_24 = data_0216e86c;
  data_027c027c->unk_28 = data_0216e5bc;
  data_027c027c->taskPool = &data_02175948;
  data_027c027c->taskFreeStack = data_0216eab0;
  data_027c027c->taskLists = data_0216e5f4;
  data_027c027c->scriptSlots = &data_02172c48;
  data_027c027c->unk_40 = &data_0216e5b2;
  data_027c027c->scriptSlotCurrent = &data_0216e5b6;
  data_027c027c->unk_48[0] = &data_02189cd8;
  data_027c027c->unk_48[1] = &data_02183b08;
  data_027c027c->unk_50[0] = &data_0216e708;
  data_027c027c->unk_50[1] = &data_0216e6a4;
  data_027c027c->unk_58 = data_0216e5ae;
  data_027c027c->unk_5c[0] = &data_0216e5c9;
  data_027c027c->unk_5c[1] = data_0216e5c0;
  data_027c027c->weatherFx = &data_0216edc4;
  STATE->unk_1c4 = (Unk_02080440_1c4*)&STATE->unk_184;
  STATE->mapTiles = (MapTiles*)STATE->mapTilesBuf;
  STATE->units = STATE_UNIT_SLOTS(STATE);
  STATE->players = STATE->playerSlots;
  STATE->tileCodeTerrain = STATE->tileCodeTerrainBuf;
  STATE->unk_6088 = STATE->unk_6088Buf;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
