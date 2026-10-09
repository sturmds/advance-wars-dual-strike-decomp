#include "types.h"
#include "game/sound_handle.h"

extern LinkList data_022a805c;             /* all sequence players, sorted by priority */

void func_02105240(SoundSeqPlayer *player);
void func_021052a0(LinkList *list, SoundSeqPlayer *player);

/* Gives a sequence player a new priority and moves it to the matching place
 * in the owner's list and in the list of all players. */
void SoundSeqPlayer_SetPriority(SoundSeqPlayer *player, int priority)
{
    LinkList *owner = player->ownerList;

    if (owner != NULL) {
        LinkList_Remove(owner, player);
        player->ownerList = NULL;
    }
    LinkList_Remove(&data_022a805c, player);
    player->priority = priority;
    if (owner != NULL) {
        func_021052a0(owner, player);
    }
    func_02105240(player);
}
