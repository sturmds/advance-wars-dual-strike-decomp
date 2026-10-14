#include "types.h"
#include "game/play_records.h"

extern PlayRecords gPlayRecords;

int func_02018e84(void) { return (gPlayRecords.unk_0f == 0) ? 1 : 0; }
