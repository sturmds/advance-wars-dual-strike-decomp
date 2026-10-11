#ifndef GAME_UNK_0215FE8C_H
#define GAME_UNK_0215FE8C_H

#include "types.h"

/* Pointed to by data_0215fe8c. Read by link play code (LinkPhase_InitScreen,
 * map select, battle setup) through small getters. */
typedef struct Unk_0215fe8c {
    u32 replyTimer;                    /* ticks since the peer last answered; Link_PollPeerReply sets error past 600 */
    u16 unk_04;                        /* cleared when func_0207b4e8 and func_0207b564 send a request */
    u16 unk_06;                        /* cleared by Link_ResetBuffers */
    u16 unk_08;                        /* func_0207a1dc result, stored when func_0207bb48 stops the task */
    u8 unk_0a;                         /* copy of unk_8b of the game state, taken when link mode starts */
    u8 linkState;                      /* link state, 0 to 3: func_0207b564 sets 1, func_0207b4e8 sets 3 */
    u8 unk_0c;                         /* func_0207bb48: 0 while the data_0215feb8 task runs, else 1 */
    u8 ownPlayer;                      /* this DS's link player, 0-3; 0 for the parent (Link_GetOwnPlayer) */
    u8 unk_0e;                         /* packet counter, incremented by Link_SendPacket */
    u8 packetSize;                     /* byte count Link_SendUnflaggedPacket copies into its packets */
    u8 nameSendAid;                    /* child whose name Link_SendNextPlayerName sends next, 1 to 7; 0 with no children */
    u8 pad_11[0x14 - 0x11];
    u32 replyOk;                       /* 1 when the last peer check answered, else 0 (Link_PollPeerReply) */
    u32 error;                         /* set to 1 by Link_FlagError, e.g. when the event queue is full */
    u32 linkActive;                    /* 1 between Link_Start and Link_Stop (Link_IsActive returns it) */
} Unk_0215fe8c;

#endif
