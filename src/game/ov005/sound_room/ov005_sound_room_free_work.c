#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

void func_0202244c(void *handle);
void Mem_Free(void *block);
void Ov005SoundRoom_DestroyMusicTask(void);

void Ov005SoundRoom_FreeWork(Ov005Task *task)
{
    Unk_ov005_0236bbac *work = task->work;

    Ov005SoundRoom_DestroyMusicTask();
    func_0202244c(work->mainObjects[0]);
    func_0202244c(work->mainObjects[1]);
    func_0202244c(work->mainObjects[2]);
    func_0202244c(work->mainObjects[3]);
    func_0202244c(work->subObjects[0]);
    func_0202244c(work->subObjects[1]);
    func_0202244c(work->subObjects[2]);
    func_0202244c(work->subObjects[3]);
    func_0202244c(work->subObjects[4]);
    Mem_Free(work->spriteFileA);
    Mem_Free(work->spriteFileB);
    Mem_Free(work);
}
