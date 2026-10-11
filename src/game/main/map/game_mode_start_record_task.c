#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int BattleSave_GetRecordId(int mode, int campaignSlot);
void func_020d8294(int recordId, int arg);

/* Starts func_020d8294's task for the record id that BattleSave_GetRecordId gives the
 * current game mode (campaign slot 0). */
void GameMode_StartRecordTask(void)
{
    func_020d8294(BattleSave_GetRecordId(data_027c027c->unk_48[data_027c0284]->gameMode, 0), 0);
}
