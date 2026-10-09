#ifndef GAME_AI_SITE_H
#define GAME_AI_SITE_H

#include "types.h"

/* One 12-byte record of the AI's site list, data_0229e5bc. Only the
 * fields seen so far are listed. */
typedef struct AiSite {
    u8 x;
    u8 y;
    u8 active;                         /* 1: the record is in use */
    u8 kind;                           /* 0xfd: the record is skipped */
    u8 pad_04[7];
    s8 score;                          /* set by AiSite_ScorePiperunnerNeed */
} AiSite;

typedef char AiSite_size_check[sizeof(AiSite) == 12 ? 1 : -1];

extern AiSite data_0229e5bc[];

#endif
