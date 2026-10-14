#ifndef GAME_WIRELESS_PARENT_INFO_H
#define GAME_WIRELESS_PARENT_INFO_H

#include "types.h"
#include "game/Unk_0229c960.h"

/* What a parent advertises: data_0229c8a0 points to a 0x40 byte copy of
 * data_0229c960, made at link init (Wireless_StartChannelSurvey). */
typedef Unk_0229c960 WirelessParentInfo;

extern WirelessParentInfo* data_0229c8a0;

#endif
