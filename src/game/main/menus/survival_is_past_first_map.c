#include "types.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

BOOL Survival_IsPastFirstMap(void) { return data_022a7b80.mapIndex != 0; }
