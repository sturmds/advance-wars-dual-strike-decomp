#ifndef GAME_UNIT_SHOW_ENTRY_H
#define GAME_UNIT_SHOW_ENTRY_H

#include "types.h"

#define ARMY_STYLE_COUNT 5

/* How a unit show task looks and sounds, one entry per UnitShowTask.entry
 * (26 entries). The sub screen and the main screen load different files. */
typedef struct UnitShowEntry {
    const char *subPalettePath[ARMY_STYLE_COUNT]; /* indexed by army style minus 1 */
    const char *subSpritePath;
    const char *mainPalettePath[ARMY_STYLE_COUNT];
    const char *mainSpritePath;
    s16 stepSize;                      /* UnitShowTask.stepSize, doubled in one game mode */
    u16 stepSound;                     /* UnitShowTask: played from func_ov000_022adc10 for entries 1 and 2 */
    u16 sound;                         /* Sound_PlaySe id, or SHOW_SOUND_* */
    u8 pad_36[2];
} UnitShowEntry;

extern UnitShowEntry gUnitShowEntries[];

#endif
