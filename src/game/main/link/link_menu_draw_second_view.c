#include "types.h"
#include "game/link_menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"
#include "game/link_buffers.h"

void Mem_Copy32(const void* src, void* dst, u32 size);
void Gfx_RequestLayerCopy(int layer);
void Gfx_ClearLayerMap(int layer);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
void Palette_Load(const void* src, u32 offset, u32 size);
int MainLoop_GetFrameCount(void);
s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numer, s32 denom);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int Math_SinDegrees(int degrees);
u16 Wireless_GetChildAids(void);
int func_0207a1f0(void);
int func_0207a260(void);
int func_0207a324(void);
void* Wireless_GetPlayerName(u8 index);
void func_0207c380(void);
void LinkMenu_ResetBackButton(void);
void LargeFont_DrawName(void* data, int a, u16 b, int c);

extern Unk_027c0280* data_027c0280;
extern u16 data_0229f330[];
extern char data_02167c88[];
extern const s16 gSineTable
    []; /* needed to match: const lets the compiler move the loads out of the loop */
extern u8 data_027c0040;

#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* gSineTable has 256 steps per turn; index 64 gives the cosine. */
#define SIN_0 gSineTable[0]
#define COS_0 gSineTable[64]

/* Draws the second view of the link player list (LinkMenuState.view 1;
 * switches to it from the list first). Top row: entry 0 and
 * "func_0207a1f0() + 1 / func_0207a324()". Then each entry whose bit is set
 * in Wireless_GetChildAids() (entry 0 always) gets a row, two tiles apart, starting
 * higher on screen when func_0207a324() is larger. Each entry below
 * func_0207a324() gets a sprite; the entry func_0207a260() names pulses in
 * size. Also cycles one colour (palette buffer offset 0x234) through the
 * 16 colours in data_0229fb30, one step every 4 frames. */
void LinkMenu_DrawSecondView(void) {
  int row;
  int i;
  int scale;
  u16* map; /* needed to match: read before the call in the argument list */

  switch (func_0207a324()) {
    case 8:
      row = 6;
      break;
    case 4:
    default:
      row = 10;
      break;
    case 2:
      row = 12;
      break;
  }
  if (data_0215fed0->view == 0) {
    Mem_Copy32(data_0229f330, data_027c0280->layer2Map, BG_VISIBLE_SIZE);
    Gfx_RequestLayerCopy(2);
    func_0207c380();
    LinkMenu_ResetBackButton();
    data_0215fed0->view = 1;
  }
  Gfx_ClearLayerMap(0);
  LargeFont_DrawName(Wireless_GetPlayerName(0), 1, 1, 0x200);
  map = data_027c0280->layer0Map;
  Number_Draw(28, 1, map, func_0207a1f0() + 1, 0, 0);
  Text_PrintStringImmediate(29, 1, data_027c0280->layer0Map, data_02167c88, 0,
                            0);
  map = data_027c0280->layer0Map;
  Number_Draw(30, 1, map, func_0207a324(), 0, 0);
  for (i = 0; i < LINK_AID_COUNT; i++) {
    if ((1 << i) & (Wireless_GetChildAids() | 1)) {
      LargeFont_DrawName(Wireless_GetPlayerName(i), 7, row + i * 2,
                         0x200 + i * 40);
    }
  }
  Gfx_RequestLayerCopy(0);

  for (i = 0; i < func_0207a324(); i++) {
    if ((1 << i) & (Wireless_GetChildAids() | 1)) {
      if (i == func_0207a260()) {
        scale = (Math_SinDegrees(HwMod32(MainLoop_GetFrameCount() * 16, 360)) >>
                 9) +
                0x100;
        Oam_SetAffineParams(1, HwDiv32(COS_0 * 16, DIVISOR(scale)),
                            HwDiv32(-SIN_0 * 16, DIVISOR(scale)),
                            HwDiv32(SIN_0 * 16, DIVISOR(scale)),
                            HwDiv32(COS_0 * 16, DIVISOR(scale)));
        Sprite_Queue(0, 0x220, ((((row + i * 2) << 3) - 8) & 0xff) | 0x300,
                     &data_027c0040, (i * 4) | 0x1000, 0);
      } else {
        Sprite_Queue(0, 40, ((row + i * 2) << 3) & 0xff, &data_027c0040, i * 4,
                     0);
      }
    }
  }
  Palette_Load((u16*)&data_0229fb30 + ((MainLoop_GetFrameCount() >> 2) & 15),
               0x234, 2);
}
