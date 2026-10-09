#ifndef GAME_AI_SITE_H
#define GAME_AI_SITE_H

#include "types.h"

/* One 12-byte record of the AI's site list, data_0229e5bc (count in AiBuildState.recordCount). Only the
 * fields seen so far are listed. */
typedef struct AiSite {
    u8 x;
    u8 y;
    u8 active;                         /* 1: the record is in use */
    u8 kind;                           /* 0xfd: the record is skipped; 0xfe: no action found (AiTurn_TrySiteAction); 254 and up reset to 127 by func_0209378c */
    u8 pad_04[5];
    u8 unk_09;                         /* lowest range-map value of a current player's Infantry or Mech (func_02092134) */
    u8 unk_0a;                         /* same for a unit waiting for a T-Copter that fits a Lander */
    s8 score;                          /* set by AiSite_ScorePiperunnerNeed */
} AiSite;

typedef char AiSite_size_check[sizeof(AiSite) == 12 ? 1 : -1];

extern AiSite data_0229e5bc[];

#endif
