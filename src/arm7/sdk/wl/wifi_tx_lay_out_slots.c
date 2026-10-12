#include "sdk/wl.h"
#include "sdk/mi.h"

void WifiBeacon_Build(void);
void WifiTx_DataSlotDone(WlTxFrame *frame, int sendNext);
void WifiTx_Loc3SlotDone(WlRamFrame *frame);
void WifiTx_GroupSlotDone(WlTxFrame *frame, int arg);
void WifiTx_MgmtSlotDone(WlTxFrame *frame, int arg);

#define RAM_FRAME(offset) ((WlRamFrame *)((u32)WIFI_PACKET_RAM + (offset)))
#define RAM_WORD(offset) (*(volatile u16 *)((u32)WIFI_PACKET_RAM + (offset)))
/* The two marks WifiTx_RestoreGuardWords checks, just before a frame. */
#define SET_MARKS(frameOffset)                       \
    RAM_WORD((frameOffset) - 4) = WIFI_GUARD_WORD_A; \
    RAM_WORD((frameOffset) - 2) = WIFI_GUARD_WORD_B

/*
 * Clears the seven TX slots and gives slots 0-2 (LOC1-3) their frame in
 * WIFI_PACKET_RAM and their TX done handler, at offsets that depend on the session
 * type in WlLink.unk_0c. Type 1 (beacon) also places the beacon frame in
 * slot 6 and slot 3's frame; type 2 places the two MP reply slots. The last
 * pair of marks in each layout follows the end of the TX area.
 */
void WifiTx_LayOutSlots(void)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlTxSlots *slots = WIFI_TX_LOCATIONS_OF(work);
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlStateView *view = WIFI_FROM_STATION_TABLE_OF(work);
    volatile u32 zero; /* needed to match: the fill value goes through the stack */

    zero = 0;
    Mem_FillWords_Arm7(zero, slots, 0xb4);
    slots->slot[0].busy = 0;
    slots->slot[0].queued = NULL;
    slots->slot[1].busy = 0;
    slots->slot[1].queued = NULL;
    slots->slot[2].busy = 0;
    slots->slot[2].queued = NULL;
    slots->unk_a4 = 0xffff;
    slots->savedMpReply = 0xffff;

    switch (link->unk_0c) {
    case 0:
        slots->slot[0].frame = RAM_FRAME(0x170);
        slots->slot[1].frame = RAM_FRAME(0x028);
        slots->slot[2].frame = RAM_FRAME(0x000);
        slots->slot[0].onDone = WifiTx_DataSlotDone;
        slots->slot[1].onDone = WifiTx_MgmtSlotDone;
        /* the LOC3 handler takes the WIFI_PACKET_RAM frame, not the queued one */
        slots->slot[2].onDone = (void *)WifiTx_Loc3SlotDone;
        SET_MARKS(0x028);
        SET_MARKS(0x170);
        SET_MARKS(0x794);
        link->flags_8a = 8;
        REG_W_TXREQ_SET = 1;
        break;
    case 1:
        slots->slot[0].frame = RAM_FRAME(0xaa0);
        slots->slot[1].frame = RAM_FRAME(0x958);
        slots->slot[2].frame = RAM_FRAME(0x334);
        slots->slot[0].onDone = WifiTx_DataSlotDone;
        slots->slot[1].onDone = WifiTx_MgmtSlotDone;
        slots->slot[2].onDone = WifiTx_GroupSlotDone;
        slots->slot[6].frame = RAM_FRAME(0x238);
        slots->slot[3].frame = RAM_FRAME(0x000);
        SET_MARKS(0x238);
        SET_MARKS(0x334);
        SET_MARKS(0x958);
        SET_MARKS(0xaa0);
        SET_MARKS(0x10c4);
        link->flags_8a = 0x208;
        slots->slot[6].frame = RAM_FRAME(0x238); /* the original sets it twice */
        WifiBeacon_Build();
        break;
    case 2:
        slots->slot[0].frame = RAM_FRAME(0x5d8);
        slots->slot[1].frame = RAM_FRAME(0x490);
        slots->slot[2].frame = RAM_FRAME(0x468);
        slots->slot[0].onDone = WifiTx_DataSlotDone;
        slots->slot[1].onDone = WifiTx_MgmtSlotDone;
        slots->slot[2].onDone = (void *)WifiTx_Loc3SlotDone;
        slots->slot[4].frame = RAM_FRAME(0x000);
        slots->slot[5].frame = RAM_FRAME(0x234);
        SET_MARKS(0x234);
        SET_MARKS(0x468);
        SET_MARKS(0x490);
        SET_MARKS(0x5d8);
        SET_MARKS(0xbfc);
        link->flags_8a = 0x108;
        REG_W_TXREQ_SET = 13;
        break;
    case 3:
        slots->slot[0].frame = RAM_FRAME(0x170);
        slots->slot[1].frame = RAM_FRAME(0x028);
        slots->slot[2].frame = RAM_FRAME(0x000);
        slots->slot[0].onDone = WifiTx_DataSlotDone;
        slots->slot[1].onDone = WifiTx_MgmtSlotDone;
        slots->slot[2].onDone = (void *)WifiTx_Loc3SlotDone;
        SET_MARKS(0x028);
        SET_MARKS(0x170);
        SET_MARKS(0x794);
        link->flags_8a = 0x108;
        REG_W_TXREQ_SET = 13;
        break;
    }
    if (view->state.unk_08 != 0) {
        link->flags_8a |= 0x4000;
    }
}
