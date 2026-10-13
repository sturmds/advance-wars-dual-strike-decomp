#include "types.h"
#include "game/sound_handle.h"

/* Sets how many sequence players group slotNo may run at once. */
void SoundSlot_SetMaxPlayers(int slotNo, int maxPlayers) {
  data_022a84a8[slotNo].maxPlayers = (u16)maxPlayers;
}
