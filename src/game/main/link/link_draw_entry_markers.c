#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"
#include "game/oam_cells.h"
#include "game/oam_buffer.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Palette_Load(const void* src, u32 offset, u32 size);
int MainLoop_GetFrameCount(void);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueuePicture192x160(s16 x, int y, int attr, int tile, int list);
u16 Wireless_GetChildAids(void);
int func_0207a260(void);
int func_0207a324(void);
BOOL LinkMenu_PlayerHasMap(int index);

#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* gSineTable has 256 steps per turn; index 64 gives the cosine. */
#define SIN_0 gSineTable[0]
#define COS_0 gSineTable[64]

/* Script command of a link screen, run each frame. Draws one sprite per
 * entry below func_0207a324() whose bit is set in Wireless_GetChildAids() (entry 0
 * always), two tiles apart from row 7. The entry func_0207a260() names
 * pulses in size. Entries that fail LinkMenu_PlayerHasMap use palette 2. Then cycles
 * one colour (palette buffer offset 0x234) through the 16 colours at the
 * start of gMenuState, draws two fixed sprites, and either a sprite pair
 * (data_0229fb30.unk_630 1, the named entry is not 0 and some entry passed
 * LinkMenu_PlayerHasMap) or a 192x160 picture on the sub screen (unk_630 not 1). */
void Link_DrawEntryMarkers(void) {
  int i;
  int scale;
  BOOL found;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  found = FALSE;
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
        if (LinkMenu_PlayerHasMap(i) == 1) {
          Sprite_Queue(0, 0x220, ((((7 + i * 2) << 3) - 8) & 0xff) | 0x300,
                       &data_027c0040, (i * 4) | 0x1000, 0);
          found = TRUE;
        } else {
          Sprite_Queue(0, 0x220, ((((7 + i * 2) << 3) - 8) & 0xff) | 0x300,
                       &data_027c0040, (i * 4) | 0x2000, 0);
        }
      } else {
        if (LinkMenu_PlayerHasMap(i) == 1) {
          Sprite_Queue(0, 40, ((7 + i * 2) << 3) & 0xff, &data_027c0040, i * 4,
                       0);
          found = TRUE;
        } else {
          Sprite_Queue(0, 40, ((7 + i * 2) << 3) & 0xff, &data_027c0040,
                       (i * 4) | 0x2000, 0);
        }
      }
    }
  }
  Palette_Load((u16*)&gMenuState + ((MainLoop_GetFrameCount() >> 2) & 15),
               0x234, 2);
  Sprite_Queue(0, 72, 104, &data_027c0040, 0, 0);
  Sprite_Queue(0, 168, 104, &data_027c0040, 4, 0);
  if (data_0229fb30.unk_630 == 1 && func_0207a260() != 0 && found == TRUE) {
    Sprite_Queue(0, 172, 172, &data_027c0040, 0x1000, 0);
    Sprite_Queue(0, 188, 172, &data_027c0108, 0x1000, 36);
  }
  if (data_0229fb30.unk_630 == 1) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Sprite_QueuePicture192x160(128, 126, 0x2800, 0x400, 1);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
