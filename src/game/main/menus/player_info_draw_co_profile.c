#include "types.h"
#include "game/co_data.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/text_print.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

#define MENU (gMenuState.unk_0206d67c)

/* Needed to match: the original adds the offset of coShown to gMenuState's
 * address at run time instead of folding it into the literal. */
#define SHOWN_CO_OFFSET ((u32) & ((MenuState*)0)->unk_0206d67c.coShown)
#define SHOWN_CO \
  (((u8*)&gMenuState + (MENU.player - 1) * 2 + MENU.slot)[SHOWN_CO_OFFSET])

/* First page of the player info screen: the shown CO's about text and its
 * day-to-day text on layer 0. coShown holds the CO id, with bit 7 as a flag. */
void PlayerInfo_DrawCoProfile(void) {
  Gfx_ClearLayerMap(0);
  Text_PrintId(1, 5, data_027c0280->layer0Map,
               ((TextRef*)CO_TABLE_FIELD(SHOWN_CO & 0x7f, profiles[0]))->id, 0,
               192);
  Text_PrintId(1, 16, data_027c0280->layer0Map,
               ((TextRef*)CO_TABLE_FIELD(SHOWN_CO & 0x7f, profiles[1]))->id, 0,
               448);
  Gfx_RequestLayerCopy(0);
}
