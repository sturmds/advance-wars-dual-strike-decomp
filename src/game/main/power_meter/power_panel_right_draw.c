#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;
extern u8 data_021686f0[];
extern u8 data_02168670[];
extern u8 data_0212e9b8[];
extern u8 data_021304b8[];

#define STATE (data_027c027c->unk_48[data_027c0284])
#define PANEL_CO_ID (&STATE->players[player].front)[coSlot].id

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
int Co_GetArmyPictureVariant(int coId, int army);
void TransferQueue_Add(const void *src, void *dest, int size);
BOOL PowerMeter_IsShown(int mapState);
void PowerPanelRight_QueuePowerIcon(int x, int y, int player);
void PowerPanelRight_DrawMeter(int x, int y, int player, int coSlot);

/* Draws the right power panel for one CO of a player (coSlot 0 front,
 * 1 back): the panel frame, the CO portrait copied to object tile tile in
 * palette, then the meter. While the front CO's power is on, a power icon
 * (PowerPanelRight_QueuePowerIcon) replaces the meter; the back CO's meter is left out while
 * its half of a tag power runs. Like CoPanel_Draw (ov000). */
void PowerPanelRight_Draw(int x, int y, int player, int coSlot, int tile, int palette)
{
    BOOL backPowerOn;

    Sprite_QueueAt(0, x, y, data_021686f0, 0x7010, 0);
    Sprite_QueueAt(0, x + 32, y, data_02168670, tile | (palette << 12), 0);
    Co_LoadArmyPalette(PANEL_CO_ID, palette + 16, player);
    if (Co_GetArmyPictureVariant(PANEL_CO_ID, player) == 0) {
        TransferQueue_Add(data_0212e9b8 + ((PANEL_CO_ID & PLAYER_CO_ID_MASK) - 1) * 256, data_027c0280->objVram + tile * 32, 256);
    } else {
        TransferQueue_Add(data_021304b8 + ((PANEL_CO_ID & PLAYER_CO_ID_MASK) - 1) * 256, data_027c0280->objVram + tile * 32, 256);
    }
    if (!PowerMeter_IsShown(STATE == &data_02183b08 ? MAP_STATE_FIRST : MAP_STATE_SECOND)) {
        return;
    }
    if (coSlot == 0) {
        if (STATE->players[player].front.powerState != POWER_NONE) {
            PowerPanelRight_QueuePowerIcon(x, y, player);
            return;
        }
        PowerPanelRight_DrawMeter(x, y, player, coSlot);
        return;
    }
    backPowerOn = FALSE;
    if (STATE->players[player].back.id != 0) {
        if (STATE->players[player].tagPowerHalf == TAG_POWER_BACK) {
            backPowerOn = TRUE;
        }
    }
    if (backPowerOn) {
        return;
    }
    PowerPanelRight_DrawMeter(x, y, player, coSlot);
}
