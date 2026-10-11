#include "types.h"
#include "game/screen_other_map.h"
#include "game/tile_point.h"

BOOL func_020f5848(void);
int SceneMode_IsCurrent(int a0);
void func_020f5888(int x, int y);
BOOL Unit_FindOtherMapTile(MapUnit *unit, TilePoint *pos);
void MapCursor_DrawAt(int x, int y, int style);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Per-frame callback of both Send entries of the unit menu (tables at
 * 0x021661d0 and 0x021661f0). Unless flags bit 1 is set or the other screen
 * is still scrolling, finds the tile on the other map where the acting unit
 * would arrive and, when SceneMode_IsCurrent(4) holds, scrolls the other screen to it
 * and draws the map cursor there. */
void UnitMenu_SendShowTarget(int a0, int a1, u32 flags)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *saved;
    BOOL scrolling;
    TilePoint tile;

    if (flags & 2) {
        return;
    }
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    scrolling = func_020f5848();
    Screen_SetCurrent(saved);
    Screen_SetCurrentGfx(savedGfx);
    if (scrolling) {
        return;
    }
    tile.x = STATE->actingUnitDestX;
    tile.y = STATE->actingUnitDestY;
    Unit_FindOtherMapTile(STATE->actingUnit, &tile);
    if (!SceneMode_IsCurrent(4)) {
        return;
    }
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    func_020f5888(tile.x, tile.y);
    MapCursor_DrawAt(tile.x << 4, tile.y << 4, 2);
    Screen_SetCurrent(saved);
    Screen_SetCurrentGfx(savedGfx);
}
