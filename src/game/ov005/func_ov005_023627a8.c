#include "types.h"
#include "game/Unk_ov005_0236bba8.h"

int Co_GetSavedPaletteVariant(int coId);

void func_ov005_023627a8(Unk_ov005_0236bba8 *work)
{
    work->unk_210 = Co_GetSavedPaletteVariant(work->coId & 0x7f);
    work->unk_20c = work->unk_210;
    if (work->unk_20c != 8) {
        work->unk_214 = work->unk_20c;
    } else {
        work->unk_214 = 0;
    }
}
