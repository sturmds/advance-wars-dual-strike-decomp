#ifndef GAME_SURVIVAL_RECORD_H
#define GAME_SURVIVAL_RECORD_H

#include "types.h"

/* 12-byte entries from data_02291588, indexed by Survival_GetRecordIndex (probably
 * one per Survival map). Each mode pair (data_022a7b80.mode 0/3,
 * 1/4, 2/5) keeps the two COs of its record here, 5 bits each; 0: none. */
typedef struct SurvivalRecordCos {
  u32 days : 10;   /* best Days Survival result (modes 2 and 5) */
  u32 mode0Co : 5; /* modes 0 and 3 */
  u32 mode0TagCo : 5;
  u32 mode2Co : 5; /* modes 2 and 5 */
  u32 mode2TagCo : 5;
  u32 unk_00_30 : 2;
  u32 unk_04; /* see SurvivalRecordCos1 */
  u32 unk_08;
} SurvivalRecordCos;

/* Needed to match: the original reaches the second word through its own
 * symbol, data_0229158c. */
typedef struct SurvivalRecordCos1 {
  u32 mode1Co : 5; /* modes 1 and 4 */
  u32 mode1TagCo : 5;
  u32 unk_04_10 : 22;
  u32 unk_08;
  u32 unk_0c;
} SurvivalRecordCos1;

/* The same 12-byte entries from their first word, data_02291584. */
typedef struct SurvivalRecordFunds {
  u32 fundsSpent
      : 24; /* best Funds Survival result (modes 1 and 4): funds spent */
  u32 : 8;
  u32 unk_04;
  u32 unk_08;
} SurvivalRecordFunds;

extern SurvivalRecordFunds data_02291584[];
extern SurvivalRecordCos data_02291588[];
/* Best Time Survival result (modes 0 and 3) per entry, in seconds. */
extern u16 data_02291650[];
extern SurvivalRecordCos1 data_0229158c[];

#endif
