#include "types.h"
#include "game/task.h"
#include "game/design_map.h"
#include "game/link_data_sync_task.h"

extern DesignMap data_022a6c84[];

u16 Wireless_GetChildAids(void);
u8 *Wireless_GetSharedData(u16 slot);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(u8 *chunk);
void Task_ResumeScript(Task *task);

/* Each frame, sends the next 8 bytes of this player's design maps and
 * copies the chunk each linked player sent into that player's rows of
 * data_022a6c84 (slot n fills rows 3n to 3n + 2). The first chunk holds
 * only a 1 in byte 0, to tell the others the swap has begun. When all
 * bytes are sent and received, resumes the task's script. */
void LinkDesignMapSync_Update(LinkDataSyncTask *task)
{
    u8 *dest;
    int i;
    u8 chunk[LINK_SYNC_CHUNK];
    int slot;
    u8 *src;

    if (!func_0207bd94()) {
        return;
    }
    if (task->receiveStarted == 0) {
        if (*Wireless_GetSharedData(0) != 0) {
            task->receiveStarted = 1;
        }
    } else if (task->receiveStarted != 0) { /* needed to match: tests the flag again */
        for (slot = 0; slot < 4; slot++) {
            if ((Wireless_GetChildAids() | 1) & (1 << slot)) {
                dest = (u8 *)&data_022a6c84[slot * LINK_DESIGN_MAPS_PER_PLAYER] + task->receivedBytes;
                src = Wireless_GetSharedData((u16)slot);
                for (i = task->receivedBytes; i < task->receivedBytes + LINK_SYNC_CHUNK; i++, dest++) {
                    if (i >= task->sentBytes) {
                        break;
                    }
                    *dest = src[i - task->receivedBytes];
                }
            }
        }
        task->receivedBytes += LINK_SYNC_CHUNK;
    }
    if (task->sendDone != 0 && task->receivedBytes >= task->sentBytes) {
        task->sentBytes = 0;
        task->receivedBytes = 0;
        task->sendDone = 0;
        task->started = 0;
        task->receiveStarted = 0;
        func_0207b48c();
        Task_ResumeScript((Task *)task);
        return;
    }
    if (task->started == 0) {
        chunk[0] = 1;
        for (i = 1; i < LINK_SYNC_CHUNK; i++) {
            chunk[i] = 0;
        }
        task->started = 1;
    } else if (task->sendDone == 0) {
        src = task->sendBuffer + task->sentBytes;
        for (i = 0; i < LINK_SYNC_CHUNK; i++) {
            chunk[i] = *src;
            src++;
            task->sentBytes++;
            if (task->sentBytes == LINK_DESIGN_MAP_BLOCK_SIZE) {
                task->sendDone = 1;
                i++;
                break;
            }
        }
        for (; i < LINK_SYNC_CHUNK; i++) {
            chunk[i] = 0;
        }
    } else {
        for (i = 0; i < LINK_SYNC_CHUNK; i++) {
            chunk[i] = 0;
        }
    }
    func_0207b4b0(chunk);
}
