#ifndef GAME_AI_PROFILE_H
#define GAME_AI_PROFILE_H

#include "types.h"

/* How the AI rates one unit type, inside AiProfile. Field meanings unknown. */
typedef struct AiUnitProfile {
    u8 unk_00;                         /* from AiProfileMods.units */
    u8 unk_01;                         /* AiProfileTemplate value, randomised */
    u8 unk_02;                         /* AiProfileTemplate value, randomised */
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;                         /* from AiProfileMods.units */
    s8 unk_06[7];
} AiUnitProfile;

/* The AI player's profile for this turn, built by AiProfile_Build from an
 * AiProfileTemplate and an AiProfileMods. */
typedef struct AiProfile {
    u32 unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 unk_08[12];
    AiUnitProfile units[26];           /* UNIT_INFANTRY to UNIT_OOZIUM */
} AiProfile;

/* One unit type in AiProfileTemplate. A value given as a base and a
 * spread is randomised by func_0208cdb8. */
typedef struct AiUnitProfileTemplate {
    u8 base1;
    u8 spread1;
    u8 base2;
    u8 spread2;
    u8 unk_04;
    u8 unk_05;
    s8 unk_06[7];
} AiUnitProfileTemplate;

/* 0x15c bytes; table at data_02162a90. */
typedef struct AiProfileTemplate {
    u32 unk_00;
    u8 unk_04;                         /* used without a tag partner */
    u8 unk_05;
    u8 unk_06;                         /* used with a tag partner */
    u8 unk_07;
    u8 unk_08;
    u8 unk_09;
    AiUnitProfileTemplate units[26];
} AiProfileTemplate;

/* 0x40 bytes; table at gAiProfileMods. */
typedef struct AiProfileMods {
    u8 unk_00[12];
    u8 units[26][2];                   /* per unit type: AiUnitProfile.unk_00 and unk_05 */
} AiProfileMods;

extern AiProfile *data_021619c4;
extern AiUnitProfile *data_0229e160;    /* entry of the active unit's type */

#endif
