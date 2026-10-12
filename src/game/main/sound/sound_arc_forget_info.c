#include "types.h"
#include "game/sound_arc.h"

/* Unload callback of the INFO block that SoundArc_ReadTables puts in a sound
 * heap: clears arc's pointer to it. */
void SoundArc_ForgetInfo(void* payload, u32 size, SoundArc* arc) {
  arc->info = NULL;
}
