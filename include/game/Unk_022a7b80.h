#ifndef GAME_UNK_022A7B80_H
#define GAME_UNK_022A7B80_H

#include "types.h"

/* data_022a7b80 (main bss): the Survival run in progress. */
typedef struct Unk_022a7b80 {
  u8 coIds
      [2]; /* front and back CO of the run's player; Survival_SavePlayerCos fills them from the state, Survival_LoadPlayerCos puts them back */
  u16 powerCharge
      [2]; /* starting power meter of slot 1's front and back CO, with coIds; Survival_LoadPlayerCos copies it to Unk_02080440_48.unk_aa */
  u8 mapIndex; /* position in the mode's map list (Survival_GetMapId) */
  u8 mode; /* 0-5; n and n + 3 share the budget kind: 0/3 time, 1/4 funds, 2/5 days */
  s32 budgetLeft; /* what is left of budget; Mode_PrintVariantValue shows it by mode: seconds as h:mm, or a number */
  s32 budget; /* the mode's full budget (Survival_GetModeBudget), set when a run starts */
  s32 points; /* points the win/lose screen shows in game mode 3 */
} Unk_022a7b80;

#endif
