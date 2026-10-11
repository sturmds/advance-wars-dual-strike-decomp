#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbac.h"

void Gfx_RequestLayerCopy(int layer);
BOOL Screen_IsAffineBg(int layer);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
void Gfx_ClearLayerMap(int layer);
BOOL func_01ff8ef8(int layer);
BOOL func_021100fc(u32 tick);
u32 func_02110150(void);
u32 func_02110934(void);

extern Unk_ov005_0236bbac *data_ov005_0236bbac;
extern u32 data_ov005_02367fec[];      /* map bytes of a rotation/scaling BG by screen size */
extern u32 data_ov005_02367ffc[];      /* map bytes of a text BG by screen size */

/* Two map entries side by side: one bar column is two tiles wide. */
typedef struct TilePair {
    u16 left;
    u16 right;
} TilePair;

extern TilePair data_ov005_0236b054[]; /* tiles of each bar row */

/* While music plays: draws barHeights as 16 bars, two tiles wide and up to
 * 10 tiles high, standing on row 20 of layer 1 of the data_027c0288.unk_04
 * screen. Then sets both OBJ scales from a 16-bit mask that func_02110934
 * returns: 0x100 plus 16 per set bit. With no music it clears the layer and
 * sets both scales to -1. */
void Ov005SoundRoom_DrawBarMeter(void)
{
    Unk_ov005_0236bbac *work = data_ov005_0236bbac;
    /* needed to match: i also counts the set bits, and row also holds the
     * running sum, as in the original */
    int i;
    int row;
    int height;

    if (work->musicPlaying == 0) {
        Unk_027c0280 *saved0280 = data_027c0280;
        Unk_02080440 *saved027c;

        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        Gfx_ClearLayerMap(1);
        Gfx_RequestLayerCopy(1);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        work->objScale1 = -1;
        work->objScale2 = -1;
        return;
    }
    for (i = 0; i < 16; i++) {
        work->shownBarHeights[i] = work->barHeights[i];
    }
    {
        Unk_027c0280 *saved0280 = data_027c0280;
        Unk_02080440 *saved027c;
        u16 *map;

        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        map = data_027c0280->layer1Map + 20 * 32;
        Gfx_ClearLayerMap(1);
        for (i = 0; i < 16; i++) {
            height = work->shownBarHeights[i];
            row = 0;
            for (;;) {
                TilePair *cell;

                if (row >= height) {
                    break;
                }
                row++;
                cell = (TilePair *)((u8 *)map - row * 64); /* 64 bytes per map row */
                cell[i].left = data_ov005_0236b054[10 - row].left;
                cell[i].right = data_ov005_0236b054[10 - row].right;
            }
        }
        map = data_027c0280->layer1Map;
        TileMap_AddToEntries(map,
                      func_01ff8ef8(1) ? data_ov005_02367ffc[GFX_LAYER(data_027c0280, 1)->screenSize]
                      : Screen_IsAffineBg(1) ? data_ov005_02367fec[GFX_LAYER(data_027c0280, 1)->screenSize]
                                             : data_ov005_02367fec[GFX_LAYER(data_027c0280, 1)->screenSize] * 2,
                      0x2300);
        Gfx_RequestLayerCopy(1);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }

    if (func_021100fc(func_02110150())) {
        u32 bit;
        u32 mask;

        mask = func_02110934();
        i = 0;
        bit = 0;
        row = 0;
        for (; bit < 16; bit++) {
            if ((mask >> bit) & 1) {
                row += 32;
                i++;
            }
            work->unk_1114 = row;
        }
    }
    {
        s16 sum = work->unk_1114;

        if (sum > 512) {
            sum = 512;
        }
        work->objScale2 = (sum >> 1) + 256;
        work->objScale1 = work->objScale2;
    }
}
