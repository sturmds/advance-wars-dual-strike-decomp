#include "types.h"

void func_0209c948(void);
void func_020a9854(void);
void func_020ac648(void);
void func_020ac9e8(void);
void func_020af5f8(void);
void func_020b2904(void);

/* Queue routine for APCs and T-Copters that carry nothing, set by
 * AiAction_QueueEmptyTransports. */
void AiEmptyTransport_Act(void)
{
    func_0209c948();
    func_020af5f8();
    func_020ac9e8();
    func_020ac648();
    func_020a9854();
    func_020b2904();
}
