#ifndef GAME_LINK_REACTION_QUEUE_H
#define GAME_LINK_REACTION_QUEUE_H

#include "types.h"

/* A reaction received over the link: the sender's link slot and the button. */
typedef struct LinkReaction {
  u8 slot;
  u8 button;
} LinkReaction;

/* data_0229da68: ring buffer of received reactions. The queue is empty when
 * readIndex equals writeIndex. */
typedef struct LinkReactionQueue {
  LinkReaction slots[16];
  u8 readIndex;
  u8 writeIndex;
} LinkReactionQueue;

extern LinkReactionQueue data_0229da68;
/* Frames left before this player may send another reaction; 200 after a
 * send (LinkReaction_StartCooldown). */
extern u16 gLinkReactionCooldown;

#endif
