#ifndef GAME_AI_BUILD_STATE_H
#define GAME_AI_BUILD_STATE_H

#include "types.h"

/* Work area at *data_021619c0, used by the AI unit-action-end phase
 * (AI_STATE_END_ACTIONS), which runs AiBuildState.step through ten steps.
 * Only the fields seen so far are listed. */
typedef struct AiBuildState {
  u8 recordCount; /* entries in data_0229e5bc, 12 bytes each; the loop bound of func_02092134 */
  u8 slotCount; /* entries in gAiSiteGroups; the loop bound of func_020937dc */
  u8 step; /* 0-9, advanced by AiTurn_RunEndActions; cleared by func_0208d964 */
  u8 buildStep; /* step of AiBuild_BuyAtSite: 1 scroll to the site, 2 wait and buy; cleared by func_0208d964 */
  u8 buildSite; /* index in data_0229e5bc of the site AiTurn_QueueBuildAtSite last chose to build at */
  u8 savingSkips; /* set to 2 by AiEndActions_Start; times AiTurn_PickUnitToBuild may skip the savingTarget check */
  u8 unk_06; /* 1-4; AiBuild_RateAttackerVsTarget changes Anti-Air and Cruiser ratings by it */
  u8 unk_07; /* AiTurn_RunEndActions ends the phase once it reaches 50 after step 0; steps 2 to 5 run only when nonzero */
  u8 unk_08; /* set by func_02090154 from Ai_CountUnitsOfAiClass(1) */
  u8 pad_09[3];
  s32 unk_0c; /* AiEndActions_Start sums AiUnit_GetRepairValue over the units that Unit_CanRepairAt accepts, then scales it by 3/2 (at least 1000) */
  u32 savingTarget; /* funds still missing for a planned build; cleared by AiEndActions_Start */
} AiBuildState;

/* AiBuildState.antiAirPick: which of Anti-Air and Cruiser fits the enemy
 * aircraft better (AiBuild_PickAntiAir). The _MORE values add that the enemy
 * aircraft have more hp than the current team's units that can hit planes. */
#define AI_ANTI_AIR_NONE 0
#define AI_ANTI_AIR_AA 1
#define AI_ANTI_AIR_CRUISER 2
#define AI_ANTI_AIR_AA_MORE 3
#define AI_ANTI_AIR_CRUISER_MORE 4

extern AiBuildState* data_021619c0;

/* Target groups of AiBuild_GetTargetGroup: the unit types the AI builds
 * against, in nine groups. The Megatank, the Oozium and out-of-range types
 * are in none. */
#define TARGET_GROUP_FOOT 0
#define TARGET_GROUP_LIGHT 1
#define TARGET_GROUP_TANK 2
#define TARGET_GROUP_PLANE 3
#define TARGET_GROUP_COPTER 4
#define TARGET_GROUP_BATTLESHIP 5
#define TARGET_GROUP_CRUISER 6
#define TARGET_GROUP_TRANSPORT 7 /* transports and the Carrier */
#define TARGET_GROUP_SUB 8
#define TARGET_GROUP_COUNT 9
#define TARGET_GROUP_NONE 10

#endif
