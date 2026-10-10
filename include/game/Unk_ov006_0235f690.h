#ifndef GAME_UNK_OV006_0235F690_H
#define GAME_UNK_OV006_0235F690_H

#include "types.h"

/* One 12-byte slot; bit 0 of flags is tested by func_ov006_023547bc.
 * A slot belongs to a slot button: Ov006_AddSlotButtons treats the array as
 * starting at 0x78, so this view begins 8 bytes into each slot. */
typedef struct Ov006Slot {
    s16 flags;
    u8 pad_02;
    u8 state;                          /* slide animation: 10 start in, 11 sliding in, 20 shown,
                                           30 start out, 31 sliding out, 32 and 40 done */
    u8 pad_04[8];
} Ov006Slot;

/* The slot of one slot button, as Ov006_AddSlotButtons fills it. The button
 * slides in from x = -32 (fixed point, 12 fraction bits). */
typedef struct Ov006SlotButton {
    s32 x;
    s32 speed;
    s16 flags;
    u8 unk_0a;                         /* 32 for every slot */
    u8 state;                          /* see Ov006Slot.state */
} Ov006SlotButton;

/* Overlay 6 state reached through data_ov006_0236d640. */
typedef struct Unk_ov006_0235f690 {
    u16 unk_00;                        /* flags; Ov006_OpenScreen sets 0x1000 */
    u16 unk_02;                        /* flags: 0x10 and 0x01 are tested by func_ov006_02351ac4 */
    u8 pad_04[0x0a - 0x04];
    u16 request;                       /* code set by Ov006_PostRequest (1, 2 or 4) */
    u16 unk_0c;
    s16 unk_0e;                        /* tile x that Ov006_DeleteMarkedUnit removes a unit from */
    s16 unk_10;                        /* tile y of the same */
    s16 unk_12;
    s16 unk_14;
    u8 pad_16[0x1a - 0x16];
    u16 unk_1a;
    u16 unk_1c;
    u16 unk_1e;                        /* entry index that unk_1a was set for */
    u16 unk_20;                        /* entry index that unk_1c was set for */
    u16 oldCell;                       /* cell code of the tile an edit is about to overwrite (Ov006_BreakCellBlock) */
    s8 unk_24;
    s8 unk_25;
    s8 unk_26;
    s8 unk_27;
    s8 unk_28;
    s8 unk_29;                         /* 1 or 0: which of two layouts Ov006_UpdateEntryView updates */
    s8 unk_2a;
    u8 unk_2b;                         /* result of Ov006_CountPropertyTiles, stored by Ov006_FillMapForScreen */
    u8 unk_2c[4]; /* Player_CountUnits(1..4): count of nonzero entries per slot */
    s8 hqX[4];                         /* per army 1-4: HQ tile, -1 when none (Ov006_FindHqs) */
    s8 hqY[4];
    u8 unk_38;                         /* the id Ov006_StartIdScript was called with */
    u8 unk_39;                         /* set to 32, the id of the first slot button, by Ov006_InitSlotButtons */
    u8 pad_3a;
    s8 unk_3b;                         /* frame countdown, set to 30 on a request; at 0 a script starts (Ov006_StartScriptAfterDelay) */
    u8 unk_3c;                         /* set to 5 after a unit is removed from the map */
    u8 pad_3d[0x4c - 0x3d];
    s32 chosenButton;                  /* id of the slot button that was chosen, or -1 */
    s32 unk_50;                        /* last button id of a run starting at 64, removed by func_ov006_02355b44 */
    u8 pad_54[0x5c - 0x54];
    struct Task *unk_5c;               /* task of script data_ov006_0236a218 */
    struct Task *unk_60;               /* task of script data_ov006_0236a1e0 */
    struct Task *unk_64;               /* task of script data_ov006_0236acc4 (func_ov006_02354444) */
    u8 pad_68[0x78 - 0x68];
    union {
        Ov006SlotButton slotButtons[8];
        struct {
            u8 pad_78[0x80 - 0x78];
            Ov006Slot unk_80[7];       /* an 8th slot starts at 0xd4 */
            s16 unk_d4;                /* flags of that 8th slot */
            u8 pad_d6[0xd8 - 0xd6];
        };
    };
    u8 unk_d8[0xeb - 0xd8];            /* passed to Ov006_HasNonSpace */
    u8 unk_eb;                         /* its address is passed to DesignMap_CopyName */
    u8 pad_ec[0x10c - 0xec];           /* DesignRoom_Init allocates 0x10c bytes */
} Unk_ov006_0235f690;

#endif
