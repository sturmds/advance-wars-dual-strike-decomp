#include "types.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/screen_other_map.h"

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);
void Gfx_RequestLayerCopy(int layer);
int VsSetup_GetPanelTileX(int player);

extern MenuState gMenuState;

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Tile blocks of a player panel in gMenuState, for one map and for two
 * maps. What loads them is not decompiled yet. */
#define PANEL_ONE_MAP ((u8*)MENU + 0x20)
#define PANEL_TWO_MAPS ((u8*)MENU + 0xe4)

/* Needed to match: through an inline function the compiler loads the map
 * before the VsSetup_GetPanelTileX call, as the original does. */
static inline u16* Gfx_GetLayer2Map(void) { return data_027c0280->layer2Map; }

/* Puts the panel of each player slot of the Vs setup screen on BG layer 2
 * at row 2, column VsSetup_GetPanelTileX(slot), with BG palette army colour + 2.
 * The tile numbers at panel rows 8-9, columns 2-3 get (army colour - 1) * 4
 * added, probably to pick the army's emblem. Black Hole also changes the
 * tiles at columns 0-1 and 4-5. */
void VsSetup_DrawSlotPanels(void) {
  int player;
  Unk_027c0280* saved = data_027c0280;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  for (player = 0; player < MENU->setup.unk_2e3; player++) {
    if (STATE->twoMaps == 0) {
      Bg_PutTileBlock(Gfx_GetLayer2Map(), VsSetup_GetPanelTileX(player), 2,
                      PANEL_ONE_MAP,
                      (MENU->setup.armyColours[player] + 2) << 12);
    } else {
      Bg_PutTileBlock(Gfx_GetLayer2Map(), VsSetup_GetPanelTileX(player), 2,
                      PANEL_TWO_MAPS,
                      (MENU->setup.armyColours[player] + 2) << 12);
    }
    Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 8 * 32 + 2] +=
        (MENU->setup.armyColours[player] - 1) * 4;
    Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 8 * 32 + 3] +=
        (MENU->setup.armyColours[player] - 1) * 4;
    Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 9 * 32 + 2] +=
        (MENU->setup.armyColours[player] - 1) * 4;
    Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 9 * 32 + 3] +=
        (MENU->setup.armyColours[player] - 1) * 4;
    if (MENU->setup.armyColours[player] == ARMY_BLACK_HOLE) {
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 8 * 32 + 0] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 8 * 32 + 1] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 9 * 32 + 0] += 4;
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 9 * 32 + 1] += 4;
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 8 * 32 + 4] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 8 * 32 + 5] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 9 * 32 + 4] += 4;
      Gfx_GetLayer2Map()[VsSetup_GetPanelTileX(player) + 9 * 32 + 5] += 4;
    }
  }
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrentGfx(saved);
}
