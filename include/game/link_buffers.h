#ifndef GAME_LINK_BUFFERS_H
#define GAME_LINK_BUFFERS_H

#include "types.h"

/* Link play buffers that Link_ResetBuffers resets. */

/* A packet as the senders build it. The queues hold all 24 bytes. */
typedef struct LinkPacket {
  u8 header; /* sequence number in the low 7 bits; bit 7 set for flagged packets */
  u8 type;
  u8 payload[22];
} LinkPacket;

/* LinkPacket.type values seen in the packet handlers. */
#define LINK_PACKET_STOP 1
#define LINK_PACKET_CHILD_AIDS \
  2 /* payload: connected child aid bits, low byte first */
#define LINK_PACKET_PLAYER_NAME \
  3 /* 3 to 9: name of the player with aid 1 to 7 */
#define LINK_PACKET_SHARING_START 10
#define LINK_PACKET_SHARING_STARTED 11
#define LINK_PACKET_SHARING_END 12
#define LINK_PACKET_SHARING_ENDED 13
#define LINK_PACKET_ACTION \
  14 /* 14 to 17: unit action from the player with aid 0 to 3 */
#define LINK_PACKET_REACTION 18 /* payload: LINK_REACTION_SIZE bytes */
#define LINK_REACTION_SIZE 2

/* One queued packet: a LinkPacket. */
typedef struct LinkQueueSlot {
  u8 bytes[sizeof(LinkPacket)];
} LinkQueueSlot;

/* Each queue below is a ring buffer. Its two trailing bytes are
 * the read and write indexes; the queue is empty when they
 * are equal and full when the write index is one behind the read index. */

/* Slots of each ring buffer below. */
#define LINK_SEND_QUEUE_SLOTS 8
#define LINK_UNFLAGGED_QUEUE_SLOTS 16
#define LINK_FLAGGED_QUEUE_SLOTS 4

/* data_0229ca88: packets waiting to be sent, 8 slots */
typedef struct Unk_0229ca88 {
  LinkQueueSlot slots[LINK_SEND_QUEUE_SLOTS];
  u8 readIndex;
  u8 writeIndex;
} Unk_0229ca88;

/* data_0229cdfc, 7 entries, one per peer: 16 slots */
typedef struct Unk_0229cdfc {
  LinkQueueSlot slots[LINK_UNFLAGGED_QUEUE_SLOTS];
  u8 readIndex;
  u8 writeIndex;
} Unk_0229cdfc;

/* data_0229cb4c, 7 entries, one per peer: 4 slots */
typedef struct Unk_0229cb4c {
  LinkQueueSlot slots[LINK_FLAGGED_QUEUE_SLOTS];
  u8 readIndex;
  u8 writeIndex;
} Unk_0229cb4c;

/* Peers of a link, aids 1-7. */
#define LINK_PEER_COUNT 7
/* Aids of a link: 0, the parent, then the peers. */
#define LINK_AID_COUNT (LINK_PEER_COUNT + 1)
/* Last sequence number received from each peer, 0x80 before the first. */
extern u8 gLinkLastSequence[];
/* The block Link_PollPeerReply sends through data sharing. */
#define LINK_SHARED_BLOCK_SIZE 8
/* Messages in a player's shared block: LINK_MESSAGE_SIZE bytes each, a type
 * and an argument. */
#define LINK_MESSAGE_SIZE 2
#define LINK_MESSAGE_COUNT (LINK_SHARED_BLOCK_SIZE / LINK_MESSAGE_SIZE)
extern u8 data_0229ca48[];
/* The packet Link_ReadUnflaggedPacket took from the queue. */
extern LinkPacket data_0229ca50;

#endif
