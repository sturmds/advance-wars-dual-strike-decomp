#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Gfx_RequestLayerCopy(int a);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Bg_ClearRect(void *target, int x, int y, int w, int h);
s8 Text_PrintId(s16 x, s16 y, u16 *map, u32 textId, u16 unk_30, u16 unk_32);
void func_02027864(u16 *map, int x, int y, int w, int h, int palette);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Replaces the help text band (tile rows 22-23 of layer 0) on the lower screen
 * with text textId, drawn in palette 1. tile is the last argument of Text_PrintId (callers pass 64). */
void HelpText_Show(u32 textId, int tile)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Bg_ClearRect(data_027c0280->layer0Map, 0, 22, 32, 2);
    Text_PrintId(0, 22, data_027c0280->layer0Map, textId, 0, tile);
    func_02027864(data_027c0280->layer0Map, 0, 22, 32, 2, 1);
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
