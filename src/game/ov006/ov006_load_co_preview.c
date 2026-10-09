#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/screen_other_map.h"

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern u8 gArmyPalettes[][32];

void Palette_Load(const void *src, u32 offset, u32 size);
void TransferQueue_Add(const void *src, void *dst, u32 size);
u8 *func_020c67bc(void);
int func_020c6774(int player, int index);
u8 *func_020ccbe4(int co);

#define STATE data_ov006_0236d640

/* Loads the CO id in STATE->unk_1a (low 6 bits) for display on the second
 * screen: its 4-tile icon at 0x5480 in OBJ VRAM (tile index from
 * func_020c6774, row picked by the player's army style, the sprite set), its
 * 8-tile picture (func_020ccbe4) at 0x5500, and the player's army colour (the
 * palette) in sprite palette slot 2. STATE->unk_24 is the player; 0 means
 * player 1. Names are guesses: unk_1a is a CO id by its use here. */
void Ov006_LoadCoPreview(void)
{
    int player = STATE->unk_24;
    int palette;
    int co;
    u8 *icons;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    if (player == 0) {
        player = 1;
    }
    palette = data_027c027c->unk_48[data_027c0284]->players[player].armyColour;
    co = STATE->unk_1a & 0x3f;
    icons = func_020c67bc();
    TransferQueue_Add(icons + func_020c6774(player, co) * 32,
                  (u8 *)((Unk_027c0280 *)data_027c0288.unk_04)->objVram + 0x5480, 128);
    TransferQueue_Add(func_020ccbe4(co), (u8 *)((Unk_027c0280 *)data_027c0288.unk_04)->objVram + 0x5500, 256);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Palette_Load(gArmyPalettes[palette - 1], 0x240, 32);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
