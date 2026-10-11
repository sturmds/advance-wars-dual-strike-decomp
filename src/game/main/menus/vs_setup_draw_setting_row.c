#include "types.h"
#include "game/menu_state.h"

int MainLoop_GetFrameCount(void);
s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
s16 Math_SinDegrees(int degrees);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);

extern MenuState gMenuState;
extern MenuStateFrom200 data_0229ed30;
extern s16 gSineTable[];            /* sine table, index 64 = cosine of 0 */
extern u8 data_027c0038;
extern u8 data_027c0040;
extern u8 data_027c0048;
extern u8 data_027c0060;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

#define ZOOM_DIVISOR(scale) ((scale) ? (scale) : 2)

/* OBJ tiles that VsSetup_Init loads for the setting values. Weather icons
 * start at 444, the setup.unk_2dd icons at 464 and the setup.unk_2e2 icons
 * at 480, 4 tiles each. */
#define TILE_ON 252
#define TILE_OFF 260
#define TILE_DIGIT(d) ((d) * 2 + 496)

/* Draws one row of the Vs setup settings at (x, y): the row's two sprites
 * (tiles row * 16 + 300, which pulses in size on the selected row
 * setup.settingRow, and row * 8 + 180), then the current value. Funds show as four digits with the
 * thousands digit hidden when 0; setup.unk_2de and unk_2df show as two
 * digits, or "Off" when 0. */
void VsSetup_DrawSettingRow(int row, int x, int y)
{
    if (row == MENU->setup.settingRow) {
        int scale = (Math_SinDegrees(HwMod32(MainLoop_GetFrameCount() << 4, 360)) >> 9) + 256;
        int d3 = ZOOM_DIVISOR(scale);
        int d2 = ZOOM_DIVISOR(scale);
        int d1 = ZOOM_DIVISOR(scale);
        int d0 = ZOOM_DIVISOR(scale);
        int sinValue = gSineTable[0];
        int cosValue = gSineTable[64] << 4;

        Oam_SetAffineParams(1, HwDiv32(cosValue, d0), HwDiv32(-sinValue << 4, d1), HwDiv32(sinValue << 4, d2),
                            HwDiv32(cosValue, d3));
        Sprite_Queue(2, ((x - 16) & 0x1ff) | 0x200, ((y - 16) & 0xff) | 0x300, &data_027c0048, 0x1000,
                     row * 16 + 300);
    } else {
        Sprite_QueueAt(2, x & 0x1ff, y & 0xff, &data_027c0048, 0x1000, row * 16 + 300);
    }
    Sprite_QueueAt(1, x & 0x1ff, y & 0xff, &data_027c0060, 0x1000, row * 8 + 180);

    switch (row) {
    case 0:
        if (MENU->setup.fog) {
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, TILE_ON);
        } else {
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, TILE_OFF);
        }
        break;
    case 1:
        switch (MENU->setup.unk_2dc) {
        case 0:
            Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 14) & 0xff, &data_027c0040, 0x1000, 444);
            break;
        case 1:
            Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 14) & 0xff, &data_027c0040, 0x1000, 448);
            break;
        case 2:
            Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 14) & 0xff, &data_027c0040, 0x1000, 452);
            break;
        case 3:
            Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 14) & 0xff, &data_027c0040, 0x1000, 456);
            break;
        case 4:
            Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 14) & 0xff, &data_027c0040, 0x1000, 460);
            break;
        }
        break;
    case 2:
        Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 14) & 0xff, &data_027c0040, 0x1000, MENU->setup.unk_2dd * 4 + 464);
        break;
    case 3: {
        int digit = HwMod32(HwDiv32(data_0229ed30.fundsPerProperty, 1000), 10);
        if (digit != 0) {
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000, TILE_DIGIT(digit));
        }
        digit = HwMod32(HwDiv32(data_0229ed30.fundsPerProperty, 100), 10);
        Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000, TILE_DIGIT(digit));
        Sprite_QueueAt(1, (x + 16) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000, TILE_DIGIT(0));
        Sprite_QueueAt(1, (x + 24) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000, TILE_DIGIT(0));
        break;
    }
    case 4: {
        int digit;
        if (MENU->setup.unk_2de == 0) {
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, TILE_OFF);
            break;
        }
        digit = HwMod32(HwDiv32(MENU->setup.unk_2de, 10), 10);
        if (digit != 0) {
            Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000, TILE_DIGIT(digit));
        }
        Sprite_QueueAt(1, (x + 16) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000,
                       TILE_DIGIT(HwMod32(MENU->setup.unk_2de, 10)));
        break;
    }
    case 5: {
        int digit;
        if (MENU->setup.unk_2df == 0) {
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, TILE_OFF);
            break;
        }
        digit = HwMod32(HwDiv32(MENU->setup.unk_2df, 10), 10);
        if (digit != 0) {
            Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000, TILE_DIGIT(digit));
        }
        Sprite_QueueAt(1, (x + 16) & 0x1ff, (y + 16) & 0xff, &data_027c0038, 0x1000,
                       TILE_DIGIT(HwMod32(MENU->setup.unk_2df, 10)));
        break;
    }
    case 6:
        if (MENU->setup.coEffectsOn) {
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, TILE_ON);
        } else {
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, TILE_OFF);
        }
        break;
    case 7:
        switch (MENU->setup.unk_2e1) {
        case 0:
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, TILE_OFF);
            break;
        case 1:
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, 268);
            break;
        case 2:
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, 276);
            break;
        case 3:
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, 284);
            break;
        case 4:
            Sprite_QueueAt(1, x & 0x1ff, (y + 16) & 0xff, &data_027c0060, 0x1000, 292);
            break;
        }
        break;
    case 8:
        Sprite_QueueAt(1, (x + 8) & 0x1ff, (y + 14) & 0xff, &data_027c0040, 0x1000, MENU->setup.unk_2e2 * 4 + 480);
        break;
    }
}
