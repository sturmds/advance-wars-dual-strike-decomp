#include "types.h"
#include "game/sound_handle.h"

/* Marks the handle as playing nothing. */
void NNS_SndHandleInit(SoundHandle* handle) { handle->player = NULL; }
