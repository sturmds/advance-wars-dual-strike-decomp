#ifndef GAME_OV003_EFFECT_H
#define GAME_OV003_EFFECT_H

#include "types.h"
#include "game/ov003_object.h"

/* Effects an owner can have at once in data_ov003_02369738: one group of
 * OV003_EFFECTS_PER_OWNER entries per Ov003Unit (groups 0 to 15), then one
 * group per Ov003Marker (groups 16 and up). */
#define OV003_EFFECTS_PER_OWNER 4

typedef Ov003Object Ov003Effect;

/* The OV003_EFFECTS_PER_OWNER entries of an owner. Needed to match: the
 * code indexes whole groups (0x38 bytes), not entries. */
#define OV003_EFFECT_GROUP(owner) \
  (((Ov003Effect(*)[OV003_EFFECTS_PER_OWNER])data_ov003_02369738)[owner])

#endif
