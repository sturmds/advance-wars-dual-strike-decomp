#ifndef GAME_CAMPAIGN_RECAP_H
#define GAME_CAMPAIGN_RECAP_H

#include "types.h"

struct Task;

/* One of the 28 entries at CampaignRecap.unk_7c, one per campaign mission
 * (maps 0xe0-0xfb are missions 0-27 in story order). */
typedef struct CampaignRecapEntry {
    u32 unk_00;                        /* mission number: map id - 0xe0 (func_020fed58 loads that map) */
    u32 unk_04;                        /* 0 ends the list (func_020ff9fc) */
    u8 pad_08[0x14 - 0x08];
    u32 unk_14;
} CampaignRecapEntry;

/* Work block of the data_0216a8b0 task (0x80 bytes, func_020feadc
 * allocates it and stores it in the task and in data_022a7ea4). */
typedef struct CampaignRecap {
    int blend[3];                      /* per panel: blend level 0-16, eased in by CampaignRecap_Update */
    int slide[3];                      /* per panel: offset eased from 24 to 0 */
    int number;                        /* index + 1 */
    u32 mapOffset;                     /* map id - 0xe0 of the current entry */
    u32 unk_20;
    u8 pad_24[0x30 - 0x24];
    u32 unk_30;
    struct Task *fadeTask;             /* from the func_020fcdf0 family */
    struct Task *fadeTask2;
    struct Task *songFadeTask;         /* func_020fcb08 */
    int firstStep;                     /* step CampaignRecap_Update goes to after loading an entry; 3: wait 32 frames first */
    int unk_44;                        /* scaled by 50 for unk_50 and unk_5c */
    int unk_48;                        /* scaled by -50 for unk_54 and unk_60 */
    struct Task *unk_4c;               /* task of script data_0216a898 */
    int unk_50;
    int unk_54;
    int unk_58;
    int unk_5c;
    int unk_60;
    int unk_64;
    u8 pad_68[0x74 - 0x68];
    int index;                         /* current entry in unk_7c */
    u32 count;                         /* 28 */
    CampaignRecapEntry *unk_7c;
} CampaignRecap;

/* The data_0216a8b0 task, as CampaignRecap_Update sees it. The first 0x2c
 * bytes belong to the task system (Task_Create). */
typedef struct CampaignRecapTask {
    u8 pad_00[0x2c];
    CampaignRecap *work;
    u8 pad_30[0x34 - 0x30];
    u32 step;
    u8 pad_38[0x3c - 0x38];
    int frame;
    int frames;
} CampaignRecapTask;

#endif
