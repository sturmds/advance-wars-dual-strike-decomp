#include "types.h"
#include "game/sound_handle.h"

extern SoundPlayerSlot data_022a84a8[]; /* sequence player groups */

/* Sets the hardware channels the tracks of group slotNo may use. */
void SoundSlot_SetTrackChannelMask(int slotNo, u32 mask) {
  data_022a84a8[slotNo].trackChannelMask = mask;
}
