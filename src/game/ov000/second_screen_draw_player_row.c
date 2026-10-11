#include "types.h"
#include "game/screen_other_map.h"

void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);
u16 *Bg_GetMapEntry(int layer, int x, int y);
void Number_Draw(s16 x, s16 y, u16 *map, int value, u16 a, u16 b);
void Text_PrintStringImmediate(s16 x, s16 y, u16 *map, const char *text, u16 a, u16 b);

extern char data_02167c70[];                  /* "?" */

/* Second screen, player list: clears the 8x6 tile row at (x, y) on layers 0 and 1, and
 * when inGame is set draws the army emblem (tiles from 0x200), the army colour badge
 * (from 0x20a), a property icon (0x214) in the army colour's palette, the unit count
 * and the property count. A negative property count shows as "?" (the caller passes -1
 * to hide it). Which icon is which is a guess. The third argument, the player slot,
 * is not used. */
void SecondScreen_DrawPlayerRow(int x, int y, int player, int armyStyle, int armyColour, int unitCount, int propertyCount, BOOL inGame)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;
    Unk_027c0280 *savedSubGfx;
    u16 *entry;
    int tile;

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    savedSubGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Bg_FillRect(data_027c0280->layer0Map, x, y, 8, 6, 0);
    Bg_FillRect(data_027c0280->layer1Map, x, y, 8, 6, 0);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrentGfx(savedSubGfx);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
    if (!inGame) {
        return;
    }

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    savedSubGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);

    entry = Bg_GetMapEntry(0, x + 1, y + 1);
    tile = 0x200 + (armyStyle - 1) * 2;
    entry[0] = tile | (armyColour << 12);
    entry[1] = (tile + 1) | (armyColour << 12);
    entry[32] = (tile + 32) | (armyColour << 12);
    entry[33] = (tile + 33) | (armyColour << 12);

    entry = Bg_GetMapEntry(0, x + 5, y + 2);
    tile = 0x20a + (armyColour - 1) * 2;
    entry[0] = tile | 0x6000;
    entry[1] = (tile + 1) | 0x6000;
    entry[32] = (tile + 32) | 0x6000;
    entry[33] = (tile + 33) | 0x6000;

    entry = Bg_GetMapEntry(0, x + 1, y + 3);
    entry[0] = 0x214 | ((armyColour + 6) << 12);
    entry[1] = 0x215 | ((armyColour + 6) << 12);
    entry[32] = 0x234 | ((armyColour + 6) << 12);
    entry[33] = 0x235 | ((armyColour + 6) << 12);

    Number_Draw(x + 4, y + 1, data_027c0280->layer0Map, unitCount, 0, 0);
    if (propertyCount >= 0) {
        Number_Draw(x + 4, y + 3, data_027c0280->layer0Map, propertyCount, 0, 0);
    } else {
        Text_PrintStringImmediate(x + 4, y + 3, data_027c0280->layer0Map, data_02167c70, 0, 0);
    }
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrentGfx(savedSubGfx);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
}
