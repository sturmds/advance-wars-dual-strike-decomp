#include "types.h"
#include "game/Unk_02080440.h"

extern u8* data_02297790;
extern u32 gLinkPlay;

void TransferQueue_AddClear16(void* dest, int size);
void Co_LoadLargePicture(int coMood, void* dest, int paletteSlot);
void Co_LoadArmyLargePicture(int coMood, void* dest, int paletteSlot, int army);

/* Picture slot in the layer 0 tile data: 0x480 bytes at this offset. */
#define EVENT_PICTURE_OFFSET 0x3660
#define EVENT_PICTURE_SIZE 0x480

/* Loads the CO picture for coMood into the layer 0 tile data, at EVENT_PICTURE_OFFSET.
 * 0 clears the slot. The army variant is used when gLinkPlay is set. */
void EventScript_LoadCoPicture(int coMood) {
  if (coMood == 0) {
    TransferQueue_AddClear16(data_02297790 + 0x3660, EVENT_PICTURE_SIZE);
    return;
  }
  if (gLinkPlay == 0) {
    Co_LoadLargePicture(coMood, data_02297790 + 0x3660, 1);
    return;
  }
  Co_LoadArmyLargePicture(coMood, data_02297790 + 0x3660, 1,
                          MAP_STATE->currentPlayer);
}
