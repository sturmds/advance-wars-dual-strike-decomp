#include "types.h"
#include "sdk/snd.h"

void func_0210fec0(void);
void func_0210fed4(void);
void DC_StoreRange(void *start, u32 size);

/* Clears every bank link chained in the wave archive. */
void SND_DestroyWaveArc(SoundWaveArc *arc)
{
    SoundWaveArcLink *link;
    SoundWaveArcLink *next;

    func_0210fed4();
    for (link = arc->firstLink; link != NULL; link = next) {
        next = link->next;
        link->waveArc = NULL;
        link->next = NULL;
        DC_StoreRange(link, 8);
    }
    func_0210fec0();
}
