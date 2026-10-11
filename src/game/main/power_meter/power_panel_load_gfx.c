#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_027c0280 *data_027c0280;
extern u16 data_022a7290;
extern u8 data_ov000_023503a0;              /* game language */
extern const void *const data_ov000_022f65fc[];
extern const void *const data_ov000_022f6610[];
extern const void *const data_ov000_022f6624[];
extern u16 data_0213fd2c[][16];

void TransferQueue_Add(const void *src, void *dst, u32 size);
void Palette_Load(const void *src, u32 offset, u32 size);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Loads the power panel sprites for a player: clears the panel's player,
 * queues the language's panel tiles to OBJ tile 16 (a shorter set during CO
 * select, which shows only the frame), loads the player's army palette into
 * OBJ row 7 that the panel draws with, and queues one more language block to
 * tile 104. */
void PowerPanel_LoadGfx(int player, BOOL coSelectPhase)
{
    data_022a7290 = 0;
    if (coSelectPhase) {
        TransferQueue_Add(data_ov000_022f6624[data_ov000_023503a0], data_027c0280->objVram + 0x200, 0x600);
    } else {
        TransferQueue_Add(data_ov000_022f6610[data_ov000_023503a0], data_027c0280->objVram + 0x200, 0x900);
    }
    Palette_Load(data_0213fd2c[STATE->players[player].armyColour - 1], 0x2e0, 32);
    TransferQueue_Add(data_ov000_022f65fc[data_ov000_023503a0], data_027c0280->objVram + 0xd00, 0x100);
}
