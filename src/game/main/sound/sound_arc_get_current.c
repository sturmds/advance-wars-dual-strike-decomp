#include "types.h"

extern void *gCurrentSoundArc;

void *SoundArc_GetCurrent(void) {
    return gCurrentSoundArc;
}
