#include "types.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, const void *data, int unk08, int unk0a);
void CoPicture_QueueSprite(int x, int y, int unk08, int unk0a, int list);
int VsSetup_GetPanelTileX(int player);
int VsSetup_GetPlayerOfSlot(int slot);
BOOL Skills_AreDisabled(void);
int BattleSetup_IsCursorOnCo(void);

extern MenuState gMenuState;
extern u8 data_027c0040;
extern u8 data_027c01dc;

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Queues the sprites of the Vs setup screen at the button positions of
 * VsSetup_AddMapAndCoButtons, on the screen of data_027c0288.unk_00: per
 * player slot a controller marker on each map button at y 24 (tile
 * VsSetup_GetPlayerOfSlot(slot) * 4 for a human, 16 for the AI), then at y 80 the CO
 * frame and the front and back CO pictures. With skills on, four Force Rank
 * set tabs at y 144; the set of the CO under the cursor uses tiles 20 + set * 4,
 * the others 36 + set * 4. On the other screen, four sprites with skills on. */
void VsSetup_DrawButtonSprites(void)
{
    /* Needed to match: one counter for the player loops and the set loops. */
    int player;
    int cursor;
    MenuState *shifted;
    Unk_027c0280 *saved = data_027c0280;
    Unk_027c0280 *saved2;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    if (STATE->twoMaps == 0) {
        for (player = 0; player < MENU->setup.unk_2e3; player++) {
            if (MENU->setup.controllers[player][0] != PLAYER_CONTROLLER_AI) {
                Sprite_QueueAt(1, (VsSetup_GetPanelTileX(player) * 8 + 16) & 0x1ff, 24, &data_027c0040, 0, VsSetup_GetPlayerOfSlot(player + 1) * 4);
            } else {
                Sprite_QueueAt(1, (VsSetup_GetPanelTileX(player) * 8 + 16) & 0x1ff, 24, &data_027c0040, 0, 16);
            }
        }
    } else {
        for (player = 0; player < MENU->setup.unk_2e3; player++) {
            if (MENU->setup.controllers[player][0] != PLAYER_CONTROLLER_AI) {
                Sprite_QueueAt(0, (VsSetup_GetPanelTileX(player) * 8 + 4) & 0x1ff, 24, &data_027c0040, 0, VsSetup_GetPlayerOfSlot(player + 1) * 4);
            } else {
                Sprite_QueueAt(0, (VsSetup_GetPanelTileX(player) * 8 + 4) & 0x1ff, 24, &data_027c0040, 0, 16);
            }
            if (MENU->setup.controllers[player][1] != PLAYER_CONTROLLER_AI) {
                Sprite_QueueAt(1, (VsSetup_GetPanelTileX(player) * 8 + 28) & 0x1ff, 24, &data_027c0040, 0, VsSetup_GetPlayerOfSlot(player + 1) * 4);
            } else {
                Sprite_QueueAt(1, (VsSetup_GetPanelTileX(player) * 8 + 28) & 0x1ff, 24, &data_027c0040, 0, 16);
            }
        }
    }
    for (player = 0; player < MENU->setup.unk_2e3; player++) {
        Sprite_QueueAt(1, (VsSetup_GetPanelTileX(player) * 8) & 0x1ff, 80, &data_027c01dc, 0x1000, 0x204);
        if (MENU->setup.cos[player][0] != 0) {
            CoPicture_QueueSprite((VsSetup_GetPanelTileX(player) * 8) & 0x1ff, 80, (player * 2 + 2) << 12, 552 + player * 2 * 18, 2);
        }
        if (MENU->setup.cos[player][1] != 0) {
            CoPicture_QueueSprite((VsSetup_GetPanelTileX(player) * 8 + 24) & 0x1ff, 80, (player * 2 + 3) << 12, 552 + (player * 2 + 1) * 18, 2);
        }
    }
    if (!Skills_AreDisabled()) {
        if (BattleSetup_IsCursorOnCo() == 1) {
            cursor = gMenuState.setup.cursorButton - 8;
            shifted = (MenuState *)((u8 *)((u16 *)&gMenuState + (cursor >> 1)) + (cursor & 1));
            for (player = 0; player < 4; player++) {
                if (player == shifted->setup.forceRankSets[0][0]) {
                    Sprite_QueueAt(1, (88 + player * 16) & 0x1ff, 144, &data_027c0040, 0x1000, 20 + player * 4);
                } else {
                    Sprite_QueueAt(1, (88 + player * 16) & 0x1ff, 144, &data_027c0040, 0x1000, 36 + player * 4);
                }
            }
        } else {
            for (player = 0; player < 4; player++) {
                Sprite_QueueAt(1, (88 + player * 16) & 0x1ff, 144, &data_027c0040, 0x1000, 36 + player * 4);
            }
        }
    }
    Screen_SetCurrentGfx(saved);

    saved2 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    if (!Skills_AreDisabled()) {
        Sprite_QueueAt(1, 64, 72, &data_027c0040, 0x1000, 20);
        Sprite_QueueAt(1, 176, 72, &data_027c0040, 0x1000, 24);
        Sprite_QueueAt(1, 64, 120, &data_027c0040, 0x1000, 28);
        Sprite_QueueAt(1, 176, 120, &data_027c0040, 0x1000, 32);
    }
    Screen_SetCurrentGfx(saved2);
}
