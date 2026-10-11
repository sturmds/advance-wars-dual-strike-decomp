#ifndef GAME_UNIT_TARGET_ENTRY_H
#define GAME_UNIT_TARGET_ENTRY_H

#include "types.h"

/* 8-byte entry of the AI target list (data_022a6dcc); data_022a6dd4 counts them. */
typedef struct UnitTargetEntry {
    u16 unitSlot;
    u16 unk_02;
    s16 unk_04;
    s16 unk_06;
} UnitTargetEntry;

#endif
