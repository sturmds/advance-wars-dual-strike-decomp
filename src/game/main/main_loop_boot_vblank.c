#include "types.h"

int func_01ff9f0c(void);
void Link_PollPeerReply(void);
void MainLoop_FinishVBlank(void);

/* Main loop callback used during boot: polls the link peer when a frame has
 * finished (func_01ff9f0c), then runs MainLoop_FinishVBlank. */
void MainLoop_BootVBlank(void)
{
    if (func_01ff9f0c()) {
        Link_PollPeerReply();
    }
    MainLoop_FinishVBlank();
}
