#ifndef GAME_OVERLAY_H
#define GAME_OVERLAY_H

#include "types.h"

#define OVERLAY_SLOT_COUNT 7

/* per slot: 1 while its overlay is loaded */
extern u32 gOverlayLoaded[OVERLAY_SLOT_COUNT];

#endif
