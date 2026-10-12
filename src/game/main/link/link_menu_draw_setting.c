#include "types.h"
#include "game/Unk_02160ac0.h"

int MainLoop_GetFrameCount(void);
s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numer, s32 denom);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int Math_SinDegrees(int degrees);

extern s16 gSineTable[];
extern u8 data_027c0038;
extern u8 data_027c0040;
extern u8 data_027c0048;
extern u8 data_027c0060;

/* Divisor for one matrix entry: the scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Draws link menu entry `entry` (0-4) at (x, y): its icon (tile entry * 16),
 * the entry's label sprite, and below it the current setting from
 * data_02160ac0 (on/off for 0 and 2, funds as digits for 1, a number for
 * 3, one of five options for 4). With a nonzero `affine` the icon goes
 * through that OAM affine matrix and pulses in size with the frame count. */
void LinkMenu_DrawSetting(int x, int y, int entry, int affine) {
  u16 iconX;
  u16 iconY;

  if (affine) {
    int scale =
        (Math_SinDegrees(HwMod32(MainLoop_GetFrameCount() * 16, 360)) >> 9) +
        0x100;
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int sinValue;
    int cosValue;

    cosValue = gSineTable[64] * 16;
    sinValue = gSineTable[0];

    Oam_SetAffineParams(affine, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                        HwDiv32(cosValue, d3));
    iconX = ((x - 16) & 0x1ff) | (affine << 9);
    iconY = ((y - 16) & 0xff) | 0x300;
  } else {
    iconX = x & 0x1ff;
    iconY = y & 0xff;
  }
  Sprite_Queue(3, iconX, iconY, &data_027c0048, 0, entry << 4);

  switch (entry) {
    case 0:
      Sprite_Queue(2, x & 0x1ff, y & 0xff, &data_027c0060, 0, 200);
      if (data_02160ac0->unk_7f) {
        Sprite_Queue(2, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0, 164);
      } else {
        Sprite_Queue(2, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0, 172);
      }
      break;
    case 1:
      Sprite_Queue(2, x & 0x1ff, y & 0xff, &data_027c0060, 0, 208);
      Sprite_Queue(2, (x + 28) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0,
                   144);
      Sprite_Queue(2, (x + 20) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0,
                   144);
      Sprite_Queue(2, (x + 12) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0,
                   144);
      Sprite_Queue(2, (x + 4) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0,
                   144 + HwMod32(HwDiv32(data_02160ac0->unk_78, 1000), 10) * 2);
      {
        /* funds come in steps of 1000: the last three digits are fixed zeros */
        int tenThousands = HwDiv32(data_02160ac0->unk_78, 10000);
        if (tenThousands != 0) {
          Sprite_Queue(2, (x - 4) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0,
                       144 + tenThousands * 2);
        }
      }
      break;
    case 2:
      Sprite_Queue(2, x & 0x1ff, y & 0xff, &data_027c0060, 0, 216);
      if (data_02160ac0->unk_80) {
        Sprite_Queue(2, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0, 164);
      } else {
        Sprite_Queue(2, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0, 172);
      }
      break;
    case 3:
      Sprite_Queue(2, x & 0x1ff, y & 0xff, &data_027c0060, 0, 224);
      Sprite_Queue(2, (x + 12) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0,
                   144 + data_02160ac0->unk_81 * 2);
      break;
    case 4:
      Sprite_Queue(2, x & 0x1ff, y & 0xff, &data_027c0060, 0, 232);
      Sprite_Queue(2, (x + 8) & 0x1ff, (y + 16) & 0xff, &data_027c0040, 0,
                   180 + data_02160ac0->unk_83 * 4);
      break;
  }
}
