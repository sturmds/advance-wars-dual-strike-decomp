#include "types.h"

void Sound_RestoreHeapState(
    int unused); /* frees the sound heap back to its last saved level */
void Sound_LoadResidentData(void);

/* Frees the sound heap back to its last saved level, then loads the
 * resident sound data again. Called from ov002 (battle animation). */
void Sound_ReloadResidentData(void) {
  Sound_RestoreHeapState(0);
  Sound_LoadResidentData();
}
