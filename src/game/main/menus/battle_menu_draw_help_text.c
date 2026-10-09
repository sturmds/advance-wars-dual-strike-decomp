#include "types.h"
#include "game/Unk_02088d04.h"

void func_0201e5f8(int a, int b, int c, int d);
void func_02017580(const void *descriptor);
void func_01ff8a2c(u16 *map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);
void Text_PrintById(int x, int y, u16 *target, u32 text, int a, int b);

extern u8 data_021583b4[];
extern Unk_027c0280 *data_027c0280;

/* Clears a 13x6 box on layer 0 and prints the help text textId in it. The
 * callers pass the text of the battle menu item they stand for. */
void BattleMenu_DrawHelpText(u32 textId)
{
    func_0201e5f8(19, 18, 14, 6);
    func_02017580(data_021583b4);
    func_01ff8a2c(data_027c0280->layer0Map, 19, 18, 13, 6, 0);
    Gfx_RequestLayerCopy(0);
    Text_PrintById(20, 19, data_027c0280->layer0Map, textId, 0, 212);
}
