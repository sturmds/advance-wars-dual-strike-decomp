#include "types.h"
#include "sdk/snd.h"

void func_0210fec0(void);
void func_0210fed4(void);
void DC_StoreRange(void *start, u32 size);

/* Takes the four wave archive links of bank out of the archives they are
 * chained in. */
void SND_DestroyBank(SoundBank *bank)
{
    int i;
    SoundWaveArcLink *link;

    func_0210fed4();
    link = bank->waveArcs;
    for (i = 0; i < 4; i++, link++) {
        SoundWaveArc *arc = bank->waveArcs[i].waveArc;

        if (arc != NULL) {
            SoundWaveArcLink *prev = arc->firstLink;

            if (link == prev) {
                arc->firstLink = bank->waveArcs[i].next;
                DC_StoreRange(arc, 60);
            } else {
                while (prev != NULL) {
                    if (link == prev->next) {
                        break;
                    }
                    prev = prev->next;
                }
                prev->next = bank->waveArcs[i].next;
                DC_StoreRange(prev, 8);
            }
        }
    }
    func_0210fec0();
}
