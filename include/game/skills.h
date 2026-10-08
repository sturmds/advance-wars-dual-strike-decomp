#ifndef GAME_SKILLS_H
#define GAME_SKILLS_H

#include "types.h"

/* Skill ids in Player.skills, as passed to
 * CheckSkillsEquipped. Id N is bit (N - 1) & 31 of word (N - 1) >> 5.
 * 0x01-0x10 are CO skills, 0x20-0x4a are Force Ranks. Ids from
 * romhacking.net doc 890 (RadioTails); add more as code uses them. */
#define SKILL_SNOW_FIREPOWER        0x06 /* Olaf day-to-day */
#define SKILL_PARTNER_TEAMWORK      0x08 /* tag partner has Teamwork */
#define SKILL_PARTNER_SYNERGY       0x09 /* tag partner has Synergy */
#define SKILL_RANK_BRUISER          0x20
#define SKILL_RANK_BRAWLER          0x21
#define SKILL_RANK_SHARPSHOOTER     0x22
#define SKILL_RANK_SNIPER           0x23
#define SKILL_RANK_HIGH_AND_DRY     0x32
#define SKILL_RANK_ICEBREAKER       0x33
#define SKILL_RANK_SAND_SCORPION    0x34
#define SKILL_RANK_LUCK             0x47 /* +10 good luck */

BOOL CheckSkillsEquipped(const u32 *skills, int id);

#endif
