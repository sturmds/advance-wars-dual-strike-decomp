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
    u8 pad_0e[0x1a - 0x0e];
    u16 unk_1a;
    u16 unk_1c;
    u16 unk_1e;                        /* entry index that unk_1a was set for */
    u16 unk_20;                        /* entry index that unk_1c was set for */
    u16 oldCell;                       /* cell code of the tile an edit is about to overwrite (Ov006_BreakCellBlock) */
    s8 unk_24;
    s8 unk_25;
    u8 pad_26;
    s8 unk_27;
    s8 unk_28;
    s8 unk_29;                         /* 1 or 0: which of two layouts Ov006_UpdateEntryView updates */
    s8 unk_2a;
    u8 unk_2b;                         /* result of Ov006_CountPropertyTiles, stored by Ov006_FillMapForScreen */
    u8 unk_2c[4]; /* Player_CountUnits(1..4): count of nonzero entries per slot */
    u8 pad_30[0x38 - 0x30];
    u8 unk_38;                         /* the id Ov006_StartIdScript was called with */
    u8 unk_39;                         /* set to 32, the id of the first slot button, by Ov006_InitSlotButtons */
    u8 pad_3a[0x4c - 0x3a];
    s32 chosenButton;                  /* id of the slot button that was chosen, or -1 */
    u8 pad_50[0x5c - 0x50];
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
