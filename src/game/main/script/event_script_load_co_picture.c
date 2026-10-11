#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 *data_02297790;
extern u32 data_0229eaec;

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_02003b44(void *dest, int size);
void Co_LoadLargePicture(int coMood, void *dest, int paletteSlot);
void func_020023b0(int coMood, void *dest, int paletteSlot, int army);

/* Picture slot in the layer 0 tile data: 0x480 bytes at this offset. */
#define EVENT_PICTURE_OFFSET 0x3660
#define EVENT_PICTURE_SIZE   0x480

/* Loads the CO picture for coMood into the layer 0 tile data, at EVENT_PICTURE_OFFSET.
 * 0 clears the slot. The army variant is used when data_0229eaec is set. */
void EventScript_LoadCoPicture(int coMood)
{
    if (coMood == 0) {
        func_02003b44(data_02297790 + 0x3660, EVENT_PICTURE_SIZE);
        return;
    }
    if (data_0229eaec == 0) {
        Co_LoadLargePicture(coMood, data_02297790 + 0x3660, 1);
        return;
    }
    func_020023b0(coMood, data_02297790 + 0x3660, 1, STATE->currentPlayer);
}
