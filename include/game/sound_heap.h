#ifndef GAME_SOUND_HEAP_H
#define GAME_SOUND_HEAP_H

#include "types.h"
#include "game/link_list.h"

/* An item loaded into the sound heap (a sequence archive, bank or wave
 * archive). onUnload runs when the heap level holding the item is dropped. */
typedef struct SoundHeapItem {
  u8 pad_00[8];
  u32 unk_08;
  void (*onUnload)(void* payload, u32 arg1, u32 arg2, u32 arg3); /* 0x0c */
  u32 unk_10;
  u32 unk_14;
  u8 pad_18[8];
  u8 payload[1]; /* 0x20: first argument of onUnload */
} SoundHeapItem;

/* NitroSystem sound heap (NNSSndHeap). Each save point is one LinkList
 * of the items loaded since the previous save point; levels is the list of
 * those save points. */
typedef struct SoundHeap {
  void* frameHeap; /* memory the items live in */
  LinkList levels; /* 0x04: levels.count is the number of levels */
} SoundHeap;

BOOL SoundHeap_AddLevel(SoundHeap* heap);

#endif
