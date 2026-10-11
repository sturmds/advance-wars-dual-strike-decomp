#include "types.h"
#include "game/sound_handle.h"

void func_021041bc(LinkList *list, void *before, void *object);

/* Adds a player to a group, after the players of equal or lower priority. */
void SoundPlayerSlot_AddPlayer(SoundPlayerSlot *slot, SoundSeqPlayer *player)
{
    SoundSeqPlayer *next = LinkList_GetNext(&slot->players, NULL);

    if (next != NULL) {
        do {
            if (player->priority < next->priority) {
                break;
            }
            next = LinkList_GetNext(&slot->players, next);
        } while (next != NULL);
    }
    func_021041bc(&slot->players, next, player);
    player->ownerSlot = slot;
}
