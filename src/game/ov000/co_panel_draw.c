#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern u8 data_ov000_0234ebf0[];
extern u8 data_0212e9b8[];
extern u8 data_021304b8[];

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
int Co_GetArmyPictureVariant(int coId, int army);
void TransferQueue_Add(const void *src, void *dest, int size);
BOOL PowerMeter_IsShown(int screen);
void PowerMeter_Draw(int x, int y, int player, int kind, int palette, int flag);
void PlayerHud_QueuePowerIcon(int x, int y, int player, int palette);

static inline BOOL Player_BackCoPowerIsOn(Unk_02080440_48 *state, int player)
{
    BOOL on = FALSE;

    if (state->players[player].back.id != 0) {
        on = state->players[player].tagPowerHalf == TAG_POWER_BACK;
    }
    return on;
}

/* Draws a CO portrait with its power meter beside it. coSlot is 0 for the
 * front CO and 1 for the back CO of the player. The portrait picture is
 * copied to object tile tile and drawn with portraitPalette. The meter is
 * left out when the back CO's power is on (the front CO's meter shows the
 * tag power) unless frontMeter is set, which makes the meter code read the
 * front CO's charge. altMeter picks PlayerHud_QueuePowerIcon for the meter. */
void CoPanel_Draw(int x, int y, int player, int coSlot, int coId, int altMeter, int screen, int tile, int portraitPalette, int meterPalette, int frontMeter)
{
    Sprite_QueueAt(0, x + 32, y, data_ov000_0234ebf0, tile | (portraitPalette << 12), 0);
    Co_LoadArmyPalette(coId, portraitPalette + 16, player);
    if (Co_GetArmyPictureVariant(coId, player) == 0) {
        TransferQueue_Add(data_0212e9b8 + ((coId & 127) - 1) * 256, data_027c0280->objVram + tile * 32, 256);
    } else {
        TransferQueue_Add(data_021304b8 + ((coId & 127) - 1) * 256, data_027c0280->objVram + tile * 32, 256);
    }
    if (!PowerMeter_IsShown(screen)) {
        return;
    }
    if (coSlot == 0 || coSlot != 1) {
        if (altMeter != 0) {
            PlayerHud_QueuePowerIcon(x, y, player, meterPalette);
            return;
        }
        PowerMeter_Draw(x, y, player, coSlot, meterPalette, 0);
        return;
    }
    if (frontMeter != 0) {
        if (altMeter != 0) {
            PlayerHud_QueuePowerIcon(x, y, player, meterPalette);
            return;
        }
        PowerMeter_Draw(x, y, player, coSlot, meterPalette, frontMeter);
        return;
    }
    if (Player_BackCoPowerIsOn(data_027c027c->unk_48[data_027c0284], player)) {
        return;
    }
    PowerMeter_Draw(x, y, player, coSlot, meterPalette, frontMeter);
}
