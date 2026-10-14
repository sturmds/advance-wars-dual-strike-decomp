#ifndef GAME_UNK_0215FE88_H
#define GAME_UNK_0215FE88_H

#include "types.h"

/* Pointed to by data_0215fe88 (main data). State of the wireless channel
 * survey that picks the quietest channel before a parent starts. */
typedef struct Unk_0215fe88 {
  u16 allowedChannels; /* bit n set while channel n + 1 may be used */
  u8 bestChannel;      /* 0 until a measurement succeeds */
  u8 channel;          /* channel being measured, 1 to 14 */
  u8 bestBusyRatio;    /* lowest busy percentage seen; 101 before the first */
  u8 pad_05[0x08 - 0x05];
  int mode; /* 1 when Wireless_StartChannelSurvey was started with 1 */
} Unk_0215fe88;

extern Unk_0215fe88* data_0215fe88;

extern Unk_0215fe88 data_0229c8c0; /* what data_0215fe88 points to */

#endif
