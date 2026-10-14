#ifndef GAME_UNK_022A7FAC_H
#define GAME_UNK_022A7FAC_H

#include "types.h"

#define MAIN_MENU_RECORD_COUNT 12

/* Main bss state at data_022a7fac: one signed byte per save record id (see
 * BattleSave_GetRecordId), 1 when MainMenu_Prepare finds that record can be
 * continued. Cleared together by MainMenu_Prepare and read one by one by
 * small getters. */
typedef struct Unk_022a7fac {
  s8 byRecord[MAIN_MENU_RECORD_COUNT];
} Unk_022a7fac;

#endif
