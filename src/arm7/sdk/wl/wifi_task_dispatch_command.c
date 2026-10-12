#include "sdk/wl.h"

/* Command tables, by command number: 0x000-0x0ff, 0x100-, 0x200-0x23f,
   0x240-0x27f, 0x280-0x2bf, 0x2c0-0x2ff and 0x300-. */
extern WlCommandEntry gWlCommands000[];
extern WlCommandEntry gWlCommands100[];
extern WlCommandEntry gWlCommands200[];
extern WlCommandEntry gWlCommands240[];
extern WlCommandEntry gWlCommands280[];
extern WlCommandEntry gWlCommands2c0[];
extern WlCommandEntry gWlCommands300[];

/*
 * Task 11: takes the first request from the inbox, checks it, and runs its
 * handler from the command table. The high byte of the command (and, for
 * 0x2xx, bits 6-7) picks the table and the bit set in current->busy while it
 * runs. The buffer goes back to the host unless the handler keeps it.
 */
void WifiTask_DispatchCommand(void)
{
    u8 result;
    WlTxRequest *request;
    WlCurrentRequest *current;
    WlCommandEntry *table;
    u16 command;
    WlReply *reply;
    u16 index;
    u16 last;
    u8 busyBit;

    current = &WIFI_DRIVER_STATE->state.current;
    result = 0;
    busyBit = 0;

    if (current->busy != 0) {
        return;
    }
    current->buffer = WIFI_DRIVER_STATE->inbox.first;
    if (current->buffer == WIFI_LIST_END) {
        return;
    }
    request = (WlTxRequest *)current->buffer;
    reply = (WlReply *)((u16 *)request + request->replyOffset + 8);

    if (WIFI_DRIVER_STATE->state.unk_12 != 0) {
        reply->status = 1;
        reply->unk_04 = 6; /* refused while any bit of WlState.unk_12 is set */
        goto done;
    }
    command = request->command;
    if (command != reply->unk_00) {
        reply->unk_04 = WIFI_ANSWER_ID_DIFFERS;
        goto done;
    }

    switch (command & 0xff00) {
    case 0x000:
        busyBit = 1;
        table = gWlCommands000;
        index = command & 0xff;
        last = 11;
        if (current->busy & 1) {
            result = WIFI_ANSWER_GROUP_BUSY;
        } else if (WIFI_DRIVER_STATE->state.mode < 32) {
            result = WIFI_ANSWER_BAD_MODE;
        }
        break;
    case 0x100:
        busyBit = 2;
        table = gWlCommands100;
        index = command & 0xff;
        last = 5;
        if (WIFI_DRIVER_STATE->state.mode != 64) {
            result = WIFI_ANSWER_BAD_MODE;
        }
        break;
    case 0x200:
        index = command & 0xff;
        if (index < 0x40) {
            if (WIFI_DRIVER_STATE->state.mode < 16) {
                result = WIFI_ANSWER_BAD_MODE;
            }
            busyBit = 4;
            table = gWlCommands200;
            last = 22;
        } else if (index < 0x80) {
            if (WIFI_DRIVER_STATE->state.mode != 64) {
                result = WIFI_ANSWER_BAD_MODE;
            }
            busyBit = 8;
            table = gWlCommands240;
            index -= 0x40;
            last = 6;
        } else if (index < 0xc0) {
            if (WIFI_DRIVER_STATE->state.mode < 16) {
                result = WIFI_ANSWER_BAD_MODE;
            }
            busyBit = 16;
            table = gWlCommands280;
            index -= 0x80;
            last = 22;
        } else {
            if (WIFI_DRIVER_STATE->state.mode < 16) {
                result = WIFI_ANSWER_BAD_MODE;
            }
            busyBit = 32;
            table = gWlCommands2c0;
            index -= 0xc0;
            last = 6;
        }
        break;
    case 0x300:
        busyBit = 64;
        table = gWlCommands300;
        index = command & 0xff;
        last = 10;
        break;
    default:
        index = 1;
        last = 0;
        break;
    }

    /* last is the table length, so one index past the end gets through */
    if (index > last) {
        result = WIFI_ANSWER_NO_SUCH_REQ;
    } else if (
        /* needed to match: replyOffset read again, through the handlers' type */
        ((WlRequest *)request)->unk_0e < table[index].requestLength
        || reply->status < table[index].replyLength) {
        result = WIFI_ANSWER_ROOM_SHORT;
    }
    if (result != 0) {
        reply->status = 1;
        reply->unk_04 = result;
        goto done;
    }

    current->busy |= busyBit;
    reply->unk_04 = table[index].handler((WlRequest *)request, reply);
    switch (reply->unk_04) {
    case WIFI_ANSWER_LATER:
        return;
    case WIFI_ANSWER_KEPT_BUFFER:
        current->busy &= ~busyBit;
        break;
    default:
    done:
        current->busy &= ~busyBit;
        WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->inbox, current->buffer);
        break;
    }
    if (WIFI_DRIVER_STATE->inbox.count != 0) {
        WifiTask_Schedule(2, 11);
    }
}
