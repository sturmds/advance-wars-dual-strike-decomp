#include "types.h"
#include "game/task.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_0235069c(void);
void func_ov006_0235eb7c(void);
void func_ov006_023564c4(void);
void func_ov006_02353f7c(void);
void func_ov006_02351aa4(void);
void func_ov006_02350560(void);
void Ov006_StartScriptAfterDelay(void);
void Task_ResumeScript(Task *task);

extern Unk_ov006_0235f690 *data_ov006_0236d640;
extern int data_0229eae4;

/* Dispatches on the request posted by
 * Ov006_PostRequest (likely a per-frame task callback; nothing calls it directly), then copies unk_0e and unk_10 to unk_12 and unk_14, clears
 * flag 2, and when flag 0x4000 is set resumes the script task unk_5c. */
void DesignRoom_Update(void)
{
    func_ov006_0235069c();
    switch (data_ov006_0236d640->request) {
    case 0:
        Ov006_StartScriptAfterDelay();
        break;
    case 1:
        func_ov006_0235eb7c();
        break;
    case 2:
        func_ov006_023564c4();
        break;
    case 3:
        func_ov006_02353f7c();
        break;
    case 5:
        func_ov006_02350560();
        break;
    case 7:
        break;
    }
    func_ov006_02351aa4();
    data_ov006_0236d640->unk_12 = data_ov006_0236d640->unk_0e;
    data_ov006_0236d640->unk_14 = data_ov006_0236d640->unk_10;
    data_ov006_0236d640->unk_00 &= ~2;
    if (data_ov006_0236d640->unk_00 & 0x4000) {
        Task_ResumeScript(data_ov006_0236d640->unk_5c);
        data_0229eae4 = 0;
    }
}
