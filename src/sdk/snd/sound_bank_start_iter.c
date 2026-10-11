#include "types.h"
#include "sdk/snd.h"

/* Position before the first note of a bank, for SoundBank_NextNote. */
SoundBankIter SoundBank_StartIter(void)
{
    SoundBankIter iter;

    iter.instrument = 0;
    iter.note = 0;
    return iter;
}
