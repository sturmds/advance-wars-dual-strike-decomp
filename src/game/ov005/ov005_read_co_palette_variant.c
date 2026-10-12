#include "types.h"
#include "game/Unk_ov005_0236bba8.h"

int Co_GetSavedPaletteVariant(int coId);

void Ov005_ReadCoPaletteVariant(Unk_ov005_0236bba8* work) {
  work->prevVariant = Co_GetSavedPaletteVariant(work->coId & 0x7f);
  work->variant = work->prevVariant;
  if (work->variant != 8) {
    work->row0Variant = work->variant;
  } else {
    work->row0Variant = 0;
  }
}
