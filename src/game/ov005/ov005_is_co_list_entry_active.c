#include "types.h"
#include "game/Unk_ov005_0236bba0.h"

int Ov005_IsCoListEntryActive(Unk_ov005_0236bba0* work, int entry) {
  return (work->entryActive[entry] != 0) ? 1 : 0;
}
