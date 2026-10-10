#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"

int Ov006_GetPropertyKind(int tile);   /* 2: HQ, 1: other property, 0: no property */
void func_ov006_02365d34(int hqTile);   /* sets that army's hqX and hqY back to -1 */

extern Unk_ov006_0235f690 *data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_AT(x, y) (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* unk_2b holds the number of property tiles (Ov006_CountPropertyTiles counts
 * them); read as signed because the code loads it with ldrsb. */
#define PROPERTY_COUNT (*(s8 *)&data_ov006_0236d640->unk_2b)

#define PROPERTY_HQ 2

/* Removes the property on tile x, y from the main screen's property list
 * (and forgets the army's HQ if it was one), then lowers the property count,
 * not below 0. Called before the tile is overwritten. */
void Ov006_RemoveProperty(int x, int y)
{
    int tile = TILE_AT(x, y);
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    int i;

    if (Ov006_GetPropertyKind(tile) == PROPERTY_HQ) {
        func_ov006_02365d34(tile);
    }
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    for (i = 0; i < 92; i++) {
        if (STATE->unk_608c[i].tile == 0xff) {
            break;
        }
        if (STATE->unk_608c[i].tile != 0 && x == STATE->unk_608c[i].x && y == STATE->unk_608c[i].y) {
            STATE->unk_6088[i].unk_00 = 0;
            STATE->unk_608c[i].tile = 0;
            PROPERTY_COUNT--;
            if (PROPERTY_COUNT < 0) {
                PROPERTY_COUNT = 0;
            }
        }
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
