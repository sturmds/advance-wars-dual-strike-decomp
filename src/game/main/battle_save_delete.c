#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void BattleSave_ClearRecordFlag(int recordId);
void func_02019920(void);
void func_02020078(int recordId);
void *func_02103c84(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Drops battle save record recordId: clears its marker byte, then
 * func_02020078 runs for the record. In game mode 0 it also calls
 * func_02103c84, which copies two words of gPlayRecords. Record 0 does
 * nothing. */
void func_02019484(int recordId)
{
    if (recordId == 0) {
        return;
    }
    BattleSave_ClearRecordFlag(recordId);
    func_02019920();
    func_02020078(recordId);
    if (STATE->gameMode == GAME_MODE_CAMPAIGN) {
        func_02103c84();
    }
}
