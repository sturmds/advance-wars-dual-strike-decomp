#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/task.h"

extern Unk_021694c0 *data_021694c0;
extern const TaskScriptCmd data_021697f8[];
extern const TaskScriptCmd data_021698a0[];
extern const TaskScriptCmd data_02169ab0[];

u32 EffectScene_GetMode(void);
void EffectScene_SetMode(int value);
void EffectScene_SparkStep(Unk_021694c0Task *task);
/* Defined with no parameters; this caller passes the task anyway. */
void EffectScene_QueueSprite(Unk_021694c0Task *task);
void Sound_PlaySe(u32 soundId);
Task *Task_FindByScript(const TaskScriptCmd *script);

/* Per-frame update of the effect scene task, by effectMode (set with
 * EffectScene_SetMode, which also clears the frame counter effectFrames). Mode 2 also draws
 * the scene sprite and plays sound 180 on frame 1 of every 600. Modes 3-5
 * start a child script on their first frame and switch the mode to 1 (mode 5:
 * to 0) once that script has ended. */
void EffectScene_Update(Unk_021694c0Task *task)
{
    switch (EffectScene_GetMode()) {
    case 1:
        EffectScene_SparkStep(task);
        return;
    case 2:
        EffectScene_SparkStep(task);
        EffectScene_QueueSprite(task);
        data_021694c0->effectFrames++;
        if (data_021694c0->effectFrames % 600 != 1) {
            return;
        }
        Sound_PlaySe(180);
        return;
    case 3:
        EffectScene_SparkStep(task);
        if (data_021694c0->effectFrames == 0) {
            Task_Create(data_021698a0, task);
        } else if (Task_FindByScript(data_021698a0) == NULL) {
            EffectScene_SetMode(1);
        }
        data_021694c0->effectFrames++;
        return;
    case 4:
        EffectScene_SparkStep(task);
        if (data_021694c0->effectFrames == 0) {
            Task_Create(data_021697f8, task);
        } else if (Task_FindByScript(data_021697f8) == NULL) {
            EffectScene_SetMode(1);
        }
        data_021694c0->effectFrames++;
        return;
    case 5:
        EffectScene_SparkStep(task);
        if (data_021694c0->effectFrames == 0) {
            Task_Create(data_02169ab0, task);
        } else if (Task_FindByScript(data_02169ab0) == NULL) {
            EffectScene_SetMode(0);
        }
        data_021694c0->effectFrames++;
        return;
    }
}
