#ifndef GAME_UNIT_SHOW_ENTRY_H
#define GAME_UNIT_SHOW_ENTRY_H

#include "types.h"

#define ARMY_STYLE_COUNT 5

/* How a unit show task looks and sounds, one entry per UnitShowTask.entry,
 * which is a unit type (26 entries). The sub screen and the main screen load different files. */
typedef struct UnitShowEntry {
  const char*
      subPalettePath[ARMY_STYLE_COUNT]; /* indexed by army style minus 1 */
  const char* subSpritePath;
  const char* mainPalettePath[ARMY_STYLE_COUNT];
  const char* mainSpritePath;
  s16 stepSize; /* UnitShowTask.stepSize, doubled unless visualMode of the game state is VISUAL_A */
  u16 stepSound; /* Sub sound effect of a step (see UnitShowTask_PlayStepSound); 0: none */
  u16 sound; /* Sound_PlaySe id, or SHOW_SOUND_* */
  u8 pad_36[2];
} UnitShowEntry;

#define SHOW_SOUND_NONE 0
#define SHOW_SOUND_STOP 0x4000 /* stop sub sound effect 20 */
#define SHOW_SOUND_SPECIAL \
  0x8000 /* UnitShowTask_PlayStepSound picks the sound */

extern UnitShowEntry gUnitShowEntries[];

#endif
