#include "types.h"
#include "game/menu_state.h"
#include "game/text_print.h"
#include "game/Unk_02088d04.h"

extern char data_02167c88[];

void Gfx_RequestLayerCopy(int layer);
void Gfx_ClearLayerMap(int layer);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
u16 Wireless_GetChildAids(void);
int func_0207a1f0(void);
int func_0207a324(void);
void* Wireless_GetPlayerName(u8 index);
void LargeFont_DrawName(void* data, int a, u16 b, int c);
int DesignMap_IsUsed(DesignMap* row);
const char* func_0207fc34(DesignMap* row);
int LinkMenu_PlayerHasMap(int player);
void HelpBand_Start(int tile, int a);
void HelpText_Show(u32 textId, int tile);

/* Draws the two-player link menu: the local player's name and the
 * "func_0207a1f0() + 1 / func_0207a324()" count on the top row, both players'
 * names, and for each player the names of the three maps in
 * MenuState.linkMenu.rows (text 0x7e000011 for an empty slot). Shows a help
 * text when either player has a map. */
void LinkMenu_DrawPlayersAndMaps(void) {
  int i;
  int player;
  int slot;
  DesignMap* row;
  u16* map;

  Gfx_ClearLayerMap(0);
  LargeFont_DrawName(Wireless_GetPlayerName(0), 1, 1, 0x200);
  map = data_027c0280->layer0Map;
  Number_Draw(28, 1, map, func_0207a1f0() + 1, 0, 0);
  Text_PrintStringImmediate(29, 1, data_027c0280->layer0Map, data_02167c88, 0,
                            0);
  map = data_027c0280->layer0Map;
  Number_Draw(30, 1, map, func_0207a324(), 0, 0);
  for (i = 0; i < LINK_MENU_PLAYER_COUNT; i++) {
    if ((1 << i) & (Wireless_GetChildAids() | 1)) {
      LargeFont_DrawName(Wireless_GetPlayerName(i), 7, 7 + i * 2,
                         0x200 + i * 40);
    }
  }
  for (player = 0; player < LINK_MENU_PLAYER_COUNT; player++) {
    for (slot = 0; slot < LINK_DESIGN_MAPS_PER_PLAYER; slot++) {
      /* Needed to match: the row rebuilt from the player's first row at each
       * slot, not stepped with row++. */
      row = &((MenuState*)&gMenuState)
                 ->linkMenu.rows[player * LINK_DESIGN_MAPS_PER_PLAYER] +
            slot;
      if (DesignMap_IsUsed(row) == 1) {
        /* Needed to match: a map local of its own, read before the call. */
        u16* layer = data_027c0280->layer0Map;
        Text_Print(5 + player * 12, 15 + slot * 2, layer, func_0207fc34(row), 0,
                   (player * 3 + slot) * 20 + 0x300);
      } else {
        Text_PrintId(5 + player * 12, 15 + slot * 2, data_027c0280->layer0Map,
                     0x7e000011, 0, (player * 3 + slot) * 20 + 0x300);
      }
    }
  }
  Gfx_RequestLayerCopy(0);
  for (i = 0; i < LINK_MENU_PLAYER_COUNT; i++) {
    if (LinkMenu_PlayerHasMap(i) == 1) {
      HelpBand_Start(0x7f8, 0);
      HelpText_Show(0x48000004, 0x3c0);
      return;
    }
  }
}
