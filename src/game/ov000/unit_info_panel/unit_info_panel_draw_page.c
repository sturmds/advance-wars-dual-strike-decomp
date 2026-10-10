#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/unit_info_panel.h"

extern Unk_027c0280 *data_027c0280;

/* Per unit type: 1-based row of data_ov000_0234fa28. */
extern s8 data_ov000_022b3850[];
/* Per row, per page: text id. */
extern u32 data_ov000_0234fa28[][16];

/* The page being shown (unk_0d holds the page drawn last). Read as signed, but
 * the field is declared u8. */
#define PANEL_PAGE (*(s8 *)&data_ov000_0234ec8c->unk_0c)

void Map_ScrollViewToTile(int mode);
void Text_StopPrinting(void);
void Gfx_RequestLayerCopy(int layer);
void Text_PrintById(s16 x, s16 y, void *target, u32 text, int a, int b);

/* Redraws the panel's text area when the page changed: clears 14 by 8 tiles
 * and prints the page's text for the unit's type. */
void func_ov000_022b2114(void)
{
    u8 row;
    u8 col;

    Map_ScrollViewToTile(2);
    if (data_ov000_0234ec8c->unk_0d == PANEL_PAGE) {
        return;
    }
    data_ov000_0234ec8c->unk_0d = PANEL_PAGE;
    Text_StopPrinting();
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 14; col++) {
            data_027c0280->layer0Map[(data_ov000_0234ec8c->tileX + 1 + col) + (data_ov000_0234ec8c->tileY + 1 + row) * 32] = 0;
        }
    }
    Gfx_RequestLayerCopy(0);
    {
        UnitInfoPanel *panel = data_ov000_0234ec8c;
        int type = panel->unit->type;
        s16 x = panel->tileX + 1;
        s16 y = panel->tileY + 1;
        Text_PrintById(x, y, data_027c0280->layer0Map, data_ov000_0234fa28[data_ov000_022b3850[type] - 1][*(s8 *)&panel->unk_0c], 0, 248);
    }
}
