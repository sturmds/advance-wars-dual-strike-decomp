#include "types.h"
#include "game/Unk_02290118.h"

BOOL Profile_IsInUse(int profile) { return data_02290a60[profile].inUse != 0; }
