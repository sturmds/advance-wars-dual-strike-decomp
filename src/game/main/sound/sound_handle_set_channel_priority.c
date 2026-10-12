#include "types.h"
#include "game/sound_handle.h"

void func_0210fe30(int hwChannel, int channelPrio);

/* Sets the priority of the hardware channel the handle's player uses. Does
 * nothing if the handle has no player. */
void SoundHandle_SetChannelPriority(SoundHandle* handle, int channelPrio) {
  if (handle->player != NULL) {
    func_0210fe30(handle->player->hwChannel, channelPrio);
  }
}
