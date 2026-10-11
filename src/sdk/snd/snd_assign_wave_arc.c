#include "types.h"
#include "sdk/snd.h"

void func_0210fec0(void);
void func_0210fed4(void);
void DC_StoreRange(void *start, u32 size);

/* Links wave archive arc into slot index of bank. Takes the link out of the
 * archive it pointed to before, if any, and flushes the changed words from the
 * data cache for the sound driver. */
void SND_AssignWaveArc(SoundBank *bank, int index, SoundWaveArc *arc)
{
    SoundWaveArc *old;
    SoundWaveArcLink *first;

    func_0210fed4();
    old = bank->waveArcs[index].waveArc;
    if (old != NULL) {
        SoundWaveArcLink *prev;

        if (arc == old) {
            func_0210fec0();
            return;
        }
        prev = old->firstLink;
        if (&bank->waveArcs[index] == prev) {
            old->firstLink = bank->waveArcs[index].next;
            DC_StoreRange(bank->waveArcs[index].waveArc, 60);
        } else {
            while (prev != NULL) {
                if (&bank->waveArcs[index] == prev->next) {
                    break;
                }
                prev = prev->next;
            }
            prev->next = bank->waveArcs[index].next;
            DC_StoreRange(prev, 8);
        }
    }
    first = arc->firstLink;
    arc->firstLink = &bank->waveArcs[index];
    bank->waveArcs[index].next = first;
    bank->waveArcs[index].waveArc = arc;
    func_0210fec0();
    DC_StoreRange(bank, 60);
    DC_StoreRange(arc, 60);
}
