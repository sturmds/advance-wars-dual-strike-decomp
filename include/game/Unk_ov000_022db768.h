#ifndef GAME_UNK_OV000_022DB768_H
#define GAME_UNK_OV000_022DB768_H

#include "types.h"

/* Entry of data_ov000_022db768 (26 entries of 0x38 bytes); the object
 * picks its entry with the byte at 0x36. */
typedef struct Unk_ov000_022db768 {
  u16 sound; /* Sound_PlaySe id, or SHOW_SOUND_* */
  u8 pad_02[0x38 - 2];
} Unk_ov000_022db768;

#define SHOW_SOUND_NONE 0
#define SHOW_SOUND_STOP 0x4000 /* stop sub sound effect 20 */
#define SHOW_SOUND_SPECIAL \
  0x8000 /* UnitShowTask_PlayStepSound picks the sound */

extern Unk_ov000_022db768 data_ov000_022db768[];

/* Object that has an entry in data_ov000_022db768. */
typedef struct Unk_ov000_022db768_Owner {
  u8 pad_00[0x36];
  u8 entry; /* index into data_ov000_022db768 */
} Unk_ov000_022db768_Owner;

#endif
