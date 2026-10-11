#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int func_02019330(int mode, int campaignSlot);
void func_020d8294(int recordId, int arg);

/* Starts func_020d8294's task for the record id that func_02019330 gives the
 * current game mode (campaign slot 0). */
void GameMode_StartRecordTask(void)
{
    func_020d8294(func_02019330(data_027c027c->unk_48[data_027c0284]->gameMode, 0), 0);
}
