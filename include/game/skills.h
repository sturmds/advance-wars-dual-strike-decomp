#ifndef GAME_SKILLS_H
#define GAME_SKILLS_H

#include "types.h"
#include "game/co_data.h"

/* Skill ids in Player.skills, as passed to
 * CheckSkillsEquipped. Id N is bit (N - 1) & 31 of word (N - 1) >> 5.
 * 0x01-0x10 are CO skills, 0x20-0x4a are Force Ranks. Ids from
 * romhacking.net doc 890 (RadioTails); add more as code uses them. */
#define SKILL_HIDDEN_HP 0x01 /* Sonja day-to-day: hp hidden from other teams */
#define SKILL_DEPLOY_CITY 0x02   /* Hachi Super CO Power */
#define SKILL_COUNTER_FIRST 0x03 /* Sonja Super: counterattack first */
#define SKILL_PIERCING_VISION \
  0x04 /* Sonja powers: woods and reefs do not hide units */
#define SKILL_DOUBLE_TERRAIN_STARS 0x05 /* Lash Super */
#define SKILL_SNOW_FIREPOWER 0x06       /* Olaf day-to-day */
#define SKILL_AIR_FUEL_SAVING 0x07      /* Eagle day-to-day */
#define SKILL_PARTNER_TEAMWORK 0x08     /* tag partner has Teamwork */
#define SKILL_PARTNER_SYNERGY 0x09      /* tag partner has Synergy */
#define SKILL_PARTNER_BODYGUARD 0x0a    /* tag partner has Bodyguard */
#define SKILL_DAMAGE_TO_FUNDS \
  0x0b /* Sasha Super CO Power, 50% of damage dealt becomes funds */
#define SKILL_INDIRECT_DEFENSE_20 0x0c /* Javier day-to-day */
#define SKILL_INDIRECT_DEFENSE_40 0x0d /* Javier CO Power */
#define SKILL_INDIRECT_DEFENSE_80 0x0e /* Javier Super CO Power */
#define SKILL_PROPERTY_INCOME 0x0f   /* Sasha day-to-day, +100 G per property */
#define SKILL_TERRAIN_FIREPOWER 0x10 /* Lash day-to-day */
#define SKILL_RANK_BRUISER 0x20
#define SKILL_RANK_BRAWLER 0x21
#define SKILL_RANK_SHARPSHOOTER 0x22
#define SKILL_RANK_SNIPER 0x23
#define SKILL_RANK_APC_BOOST 0x24 /* +1 transport movement */
#define SKILL_RANK_SLAM_GUARD 0x25
#define SKILL_RANK_SLAM_SHIELD 0x26
#define SKILL_RANK_SNIPE_GUARD 0x27
#define SKILL_RANK_SNIPE_SHIELD 0x28
#define SKILL_RANK_APC_GUARD 0x29
#define SKILL_RANK_MISSILE_GUARD 0x2a /* less damage from missile silos */
#define SKILL_RANK_CANNON_GUARD 0x2b  /* less damage from cannons */
#define SKILL_RANK_ROAD_RAGE 0x2c
#define SKILL_RANK_RANGER 0x2d
#define SKILL_RANK_URBAN_FIGHTER 0x2e
#define SKILL_RANK_MOUNTAINEER 0x2f
#define SKILL_RANK_SEAMANSHIP 0x30
#define SKILL_RANK_BACKSTAB 0x31 /* +15 firepower while dived */
#define SKILL_RANK_HIGH_AND_DRY 0x32
#define SKILL_RANK_ICEBREAKER 0x33
#define SKILL_RANK_SAND_SCORPION 0x34
#define SKILL_RANK_TEAMWORK \
  0x35 /* lends SKILL_PARTNER_TEAMWORK to the front CO */
#define SKILL_RANK_SYNERGY 0x36   /* lends SKILL_PARTNER_SYNERGY */
#define SKILL_RANK_BODYGUARD 0x37 /* lends SKILL_PARTNER_BODYGUARD */
#define SKILL_PRAIRIE_DOG 0x38
#define SKILL_PATHFINDER 0x39
#define SKILL_RANK_TOWER_POWER 0x3a /* +5 firepower per Com Tower */
#define SKILL_RANK_SCOUT 0x3b       /* +1 vision */
#define SKILL_RANK_EAGLE_EYE 0x3c   /* +2 vision */
#define SKILL_RANK_MECHANIC 0x3d    /* +1 repair */
#define SKILL_RANK_GEAR_HEAD 0x3e   /* +2 repair */
#define SKILL_RANK_INVADER 0x3f     /* +1 capture */
#define SKILL_RANK_CONQUEROR 0x40   /* +2 capture */
#define SKILL_RANK_SNEAKY 0x41
#define SKILL_RANK_STEALTHY 0x42
#define SKILL_RANK_SALE_PRICE 0x43 /* units cost 5% less */
#define SKILL_RANK_FIRE_SALE 0x44  /* units cost 8% less */
#define SKILL_RANK_COMBAT_PAY 0x45 /* 2% of damage dealt becomes funds */
#define SKILL_RANK_GOLD_RUSH 0x46  /* +100 G per property */
#define SKILL_RANK_LUCK 0x47       /* +10 good luck */
#define SKILL_RANK_STAR_POWER 0x48 /* +10% power meter charge */
#define SKILL_RANK_MISTWALKER 0x49 /* doc 890 name; CounterFirst checks it */
#define SKILL_RANK_SOUL_OF_HACHI 0x4a

BOOL CheckSkillsEquipped(const u32* skills, int id);
void PartnerBoost2(u32* skills, int id); /* turns skill id on */

/* Equipped skills of each CO, per Force Rank set: 4 words of skill bits. */
extern u32 gCoEquippedSkills[CO_COUNT][4][4];

#endif
