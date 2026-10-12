#include "types.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/screen_other_map.h"

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);
void Gfx_RequestLayerCopy(int layer);
int VsSetup_GetSlotTileX(int slot);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Tile block from res_versus that VsSetup_Init copies to gMenuState + 0x21c. */
#define TEAM_PANEL_BLOCK ((u8*)MENU + 0x21c)

/* Needed to match: through an inline function the compiler loads the map
 * before the VsSetup_GetSlotTileX call, as the original does. */
static inline u16* Gfx_GetLayer2Map(void) { return data_027c0280->layer2Map; }

/* Like VsSetup_DrawSlotPanels, on the other screen (data_027c0288.unk_04):
 * puts the block at gMenuState + 0x21c for each player slot on BG layer 2 at
 * row 8, column VsSetup_GetSlotTileX(slot), with BG palette army colour + 2.
 * Block rows 0-1, columns 3-4 get (army colour - 1) * 4 added; Black Hole
 * also changes columns 1-2 and 5-6. */
void VsSetup_DrawSubSlotPanels(void) {
  int slot;
  Unk_027c0280* saved = data_027c0280;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (slot = 0; slot < MENU->setup.unk_2e3; slot++) {
    Bg_PutTileBlock(Gfx_GetLayer2Map(), VsSetup_GetSlotTileX(slot), 8,
                    TEAM_PANEL_BLOCK,
                    (MENU->setup.armyColours[slot] + 2) << 12);
    Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 8 * 32 + 3] +=
        (MENU->setup.armyColours[slot] - 1) * 4;
    Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 8 * 32 + 4] +=
        (MENU->setup.armyColours[slot] - 1) * 4;
    Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 9 * 32 + 3] +=
        (MENU->setup.armyColours[slot] - 1) * 4;
    Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 9 * 32 + 4] +=
        (MENU->setup.armyColours[slot] - 1) * 4;
    if (MENU->setup.armyColours[slot] == ARMY_BLACK_HOLE) {
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 8 * 32 + 1] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 8 * 32 + 2] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 9 * 32 + 1] += 4;
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 9 * 32 + 2] += 4;
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 8 * 32 + 5] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 8 * 32 + 6] += 5;
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 9 * 32 + 5] += 4;
      Gfx_GetLayer2Map()[VsSetup_GetSlotTileX(slot) + 9 * 32 + 6] += 4;
    }
  }
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrentGfx(saved);
}
