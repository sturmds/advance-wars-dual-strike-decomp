#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/song.h"
#include "game/screen.h"

#define REG_DISPCNT (*(volatile u32*)0x04000000)

void Ov004_LoadRikiishiTextures(Unk_ov004_022bf158* state);
void Ov004_ResetControl(Unk_ov004_022bf158* state);
void Ov004_ClearSlots(Unk_ov004_022bf158* state);
void Ov004_ResetBg3Params(Unk_ov004_022bf158* state);
void Ov004_LoadResources(Unk_ov004_022bf158* state);
void Ov004_SetupPerspective3d(void);
void Ov004_LoadResourcePalette(const char* name, int dataOffset,
                               int paletteOffset);
void func_ov004_022bb38c(Unk_ov004_022bf158* state);
void Ov004_ResetClock(Unk_ov004_022bf158* state);

extern GfxLayer data_ov004_022be868[];
extern GfxLayer data_ov004_022be9a8[];
extern char data_ov004_022bde54[];
extern char data_ov004_022bde64[];
extern char data_ov004_022bde74[];
extern char data_ov004_022bde84[];
extern char data_ov004_022bde94[];
extern char data_ov004_022bdea4[];
extern char data_ov004_022bdeb4[];
extern char data_ov004_022bdec4[];

/* Shows BG0 to BG3 and OBJ on the current screen. */
#define SHOW_ALL_LAYERS()              \
  GFX_DISPCNT(data_027c0280)->bg0 = 1; \
  GFX_DISPCNT(data_027c0280)->bg1 = 1; \
  GFX_DISPCNT(data_027c0280)->bg2 = 1; \
  GFX_DISPCNT(data_027c0280)->bg3 = 1; \
  GFX_DISPCNT(data_027c0280)->obj = 1

void Ov004Task_InitScene(Ov004Task* task) {
  Unk_ov004_022bf158* state = task->state;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(1);
  Screen_InitLayers(data_ov004_022be868);
  SHOW_ALL_LAYERS();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov004_022be9a8);
  SHOW_ALL_LAYERS();
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  REG_DISPCNT &= ~0x7000000;
  Ov004_ResetControl(state);
  Ov004_ClearSlots(state);
  Ov004_ResetBg3Params(state);
  Sound_StopSong();
  task->step = 10;
  Ov004_LoadResources(state);
  Ov004_LoadRikiishiTextures(state);
  Ov004_SetupPerspective3d();
  Ov004_LoadResourcePalette(data_ov004_022bde84, 0x9d90, 0);
  Ov004_LoadResourcePalette(data_ov004_022bde54, 0xadac, 0x200);
  Ov004_LoadResourcePalette(data_ov004_022bde74, 0x5020, 0x400);
  Ov004_LoadResourcePalette(data_ov004_022bdec4, 0x5bf8, 0x600);
  Ov004_LoadResourcePalette(data_ov004_022bde94, 0x6060, 0x800);
  Ov004_LoadResourcePalette(data_ov004_022bde64, 0x4c1c, 0xa00);
  Ov004_LoadResourcePalette(data_ov004_022bdea4, 0x3ffc, 0xc00);
  Ov004_LoadResourcePalette(data_ov004_022bdeb4, 0x4330, 0xe00);
  func_ov004_022bb38c(state);
  Ov004_ResetClock(state);
}
