#include "types.h"
#include "game/sound_arc.h"
#include "sdk/os.h"

SoundArc* func_021066ec(SoundArc* arc);
void* SoundArc_GetFileAddress(int itemNo);
void SoundArc_SetFileAddress(int itemNo, void* loaded);

/* Run when a loaded item is unloaded: forgets that item itemNo of archive
 * arc is in memory, if loaded is where it sits. Does nothing without an
 * archive. Interrupts are off while the current archive is swapped. */
void SoundArc_ForgetLoadedItem(void* loaded, SoundArc* arc, int itemNo) {
  SoundArc* previous;
  int irqState;

  if (arc == NULL) {
    return;
  }
  irqState = OS_DisableInterrupts();
  previous = func_021066ec(arc);
  if (loaded == SoundArc_GetFileAddress(itemNo)) {
    SoundArc_SetFileAddress(itemNo, NULL);
  }
  func_021066ec(previous);
  OS_RestoreInterrupts(irqState);
}
