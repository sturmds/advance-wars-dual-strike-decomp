#include "types.h"
#include "game/co_data.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/text_print.h"

#define CO_TEXT(field) (((TextRef*)CO_TABLE_FIELD(SHOWN_CO & 0x7f, field))->id)

void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

#define MENU (gMenuState.unk_0206d67c)

/* Needed to match: the original adds the offset of coShown to gMenuState's
 * address at run time instead of folding it into the literal. */
#define SHOWN_CO_OFFSET ((u32) & ((MenuState*)0)->unk_0206d67c.coShown)
#define SHOWN_CO \
  (((u8*)&gMenuState + (MENU.player - 1) * 2 + MENU.slot)[SHOWN_CO_OFFSET])

/* Icons PlayerInfo_Init loads to layer 0 tiles 704 and 708. */
#define TILE_CO_POWER_ICON 704
#define TILE_SUPER_POWER_ICON 708

/* Third page of the player info screen: the shown CO's CO Power and Super
 * CO Power, each an icon, its name and its description, on layer 0. Von
 * Bolt has no CO Power, so his Super takes the upper place. */
void PlayerInfo_DrawPowers(void) {
  Gfx_ClearLayerMap(0);
  if ((SHOWN_CO & 0x7f) != CO_VON_BOLT) {
    Bg_PutTileQuad(10, 5, TILE_CO_POWER_ICON, 2);
    Text_PrintId(12, 5, data_027c0280->layer0Map,
                 CO_TEXT(blocks[POWER_CO].name), 0, 128);
    Text_PrintId(1, 7, data_027c0280->layer0Map, CO_TEXT(profiles[2]), 0, 192);
    Bg_PutTileQuad(10, 14, TILE_SUPER_POWER_ICON, 2);
    Text_PrintId(12, 14, data_027c0280->layer0Map,
                 CO_TEXT(blocks[POWER_SUPER].name), 0, 160);
    Text_PrintId(1, 16, data_027c0280->layer0Map, CO_TEXT(profiles[3]), 0, 448);
  } else {
    Bg_PutTileQuad(10, 5, TILE_SUPER_POWER_ICON, 2);
    Text_PrintId(12, 5, data_027c0280->layer0Map,
                 CO_TEXT(blocks[POWER_SUPER].name), 0, 128);
    Text_PrintId(1, 7, data_027c0280->layer0Map, CO_TEXT(profiles[3]), 0, 192);
  }
  Gfx_RequestLayerCopy(0);
}
