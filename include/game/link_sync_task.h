#ifndef GAME_LINK_SYNC_TASK_H
#define GAME_LINK_SYNC_TASK_H

#include "types.h"
#include "game/co_data.h"

/* One CO's entry in the experience block sent over the link (one per CO
 * id), and in each player's row of data_0229deec. */
typedef struct LinkCoRecord {
  u32 paletteVariant : 3; /* Co_GetSavedPaletteVariant */
  u32 pictureVariant : 1; /* Co_GetSavedPictureVariant */
  u32 experience : 17;    /* Co_GetExperience */
  u32 : 11;
} LinkCoRecord;

typedef char LinkCoRecord_size_check[sizeof(LinkCoRecord) == 4 ? 1 : -1];

#define LINK_SYNC_ARMY_COUNT 4 /* rows of data_0229deec and data_0229de6c */

/* Each army's CO records as received in the experience sync. */
extern LinkCoRecord data_0229deec[LINK_SYNC_ARMY_COUNT][CO_COUNT];
/* Equipped skills received in the skill sync: 16 bytes per link slot and
 * CO (LinkSkills_GetPlayerSet). */
extern u8 data_0229de6c[LINK_SYNC_ARMY_COUNT][2][16];

/* Fields of the tasks that swap a block of data between players over the
 * link, 8 bytes a frame (LinkSkillSyncTask_Update: equipped skills, LinkExperienceSyncTask_Update: CO
 * experience). Each task uses one of the two data pointers. The first 0x2a
 * bytes belong to the task system (Task_Create). */
typedef struct LinkSyncTask {
  u8 pad_00[0x2c];
  LinkCoRecord*
      experience; /* one row of data_0229deec to send, allocated by LinkExperienceSyncTask_Init */
  u8* skills; /* 128 bytes to send: 4 players x 2 COs x 16, filled by LinkSkillSyncTask_Init */
  u16 sentBytes;
  u16 receivedBytes;
  u16 sendDone;       /* set when sentBytes reaches the block size */
  u16 started;        /* set once the first chunk went out */
  u16 receiveStarted; /* set when the first chunk arrived */
} LinkSyncTask;

/* COs in data_0216069c whose variants LinkCoVariantSyncTask swaps. */
#define LINK_VARIANT_CO_COUNT 7

/* Palette and picture variants of two COs, packed in one byte. */
typedef struct LinkCoVariantPair {
  u8 palette0 : 3;
  u8 picture0 : 1;
  u8 palette1 : 3;
  u8 picture1 : 1;
} LinkCoVariantPair;

/* Fields of the task (LinkCoVariantSyncTask_Update) that swaps the palette and picture
 * variants of the 7 COs in data_0216069c between players. Its fields start
 * one byte earlier than the other tasks'. */
typedef struct LinkCoVariantSyncTask {
  u8 pad_00[0x29];
  LinkCoVariantPair variants
      [(LINK_VARIANT_CO_COUNT + 1) /
       2]; /* LINK_VARIANT_CO_COUNT COs, two a byte; filled by LinkCoVariantSyncTask_Init */
  u8 players
      [4]; /* the player in each link slot; the received chunk of player players[k] goes to army k + 1 */
  u8 pad_31;
  u16 sentBytes;
  u16 receivedBytes;
  u16 sendDone;       /* set when sentBytes reaches 4 */
  u16 started;        /* set once the first chunk went out */
  u16 receiveStarted; /* set when the first chunk arrived */
} LinkCoVariantSyncTask;

#endif
