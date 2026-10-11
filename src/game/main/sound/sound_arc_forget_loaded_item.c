#include "types.h"
#include "game/sound_arc.h"

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);
SoundArc *func_021066ec(SoundArc *arc);
void *func_02106304(int itemNo);
void func_021062e8(int itemNo, void *loaded);

/* Run when a loaded item is unloaded: forgets that item itemNo of archive
 * arc is in memory, if loaded is where it sits. Does nothing without an
 * archive. Interrupts are off while the current archive is swapped. */
void SoundArc_ForgetLoadedItem(void *loaded, SoundArc *arc, int itemNo)
{
    SoundArc *previous;
    int irqState;

    if (arc == NULL) {
        return;
    }
    irqState = OS_DisableInterrupts();
    previous = func_021066ec(arc);
    if (loaded == func_02106304(itemNo)) {
        func_021062e8(itemNo, NULL);
    }
    func_021066ec(previous);
    OS_RestoreInterrupts(irqState);
}
