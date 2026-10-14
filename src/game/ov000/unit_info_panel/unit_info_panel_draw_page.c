#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/unit_info_panel.h"

extern Unk_027c0280* data_027c0280;

/* Per unit type: 1-based row of data_ov000_0234fa28. */
extern s8 data_ov000_022b3850[];
/* Per row, per page: text id. */
extern u32 data_ov000_0234fa28[][16];

void Map_ScrollViewToTile(int mode);
void Text_StopPrinting(void);
void Gfx_RequestLayerCopy(int layer);
void Text_PrintById(s16 x, s16 y, void* target, u32 text, int a, int b);

/* Redraws the panel's text area when the page changed: clears 14 by 8 tiles
 * and prints the page's text for the unit's type. */
void UnitInfoPanel_DrawPage(void) {
  u8 row;
  u8 col;

  Map_ScrollViewToTile(2);
  if (data_ov000_0234ec8c->shownPage != data_ov000_0234ec8c->page) {
    data_ov000_0234ec8c->shownPage = data_ov000_0234ec8c->page;
    Text_StopPrinting();
    for (row = 0; row < UNIT_INFO_TEXT_ROWS; row++) {
      for (col = 0; col < UNIT_INFO_TEXT_COLUMNS; col++) {
        data_027c0280->layer0Map[(data_ov000_0234ec8c->tileX + 1 + col) +
                                 (data_ov000_0234ec8c->tileY + 1 + row) * 32] =
            0;
      }
    }
    Gfx_RequestLayerCopy(0);
    Text_PrintById(data_ov000_0234ec8c->tileX + 1,
                   data_ov000_0234ec8c->tileY + 1, data_027c0280->layer0Map,
                   data_ov000_0234fa28
                       [data_ov000_022b3850[data_ov000_0234ec8c->unit->type] -
                        1][data_ov000_0234ec8c->page],
                   0, 248);
    /* Needed to match: this return makes mwcc schedule the block again
     * after register allocation, which orders the loads for the call as in
     * the original. */
    return;
  }
}
