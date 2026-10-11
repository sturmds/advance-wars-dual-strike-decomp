#include "types.h"

int func_01ff9f0c(void);
void Link_PollPeerReply(void);
void func_01ff9bd0(void);

/* Main loop callback used during boot: polls the link peer when a frame has
 * finished (func_01ff9f0c), then runs func_01ff9bd0. */
void MainLoop_BootVBlank(void)
{
    if (func_01ff9f0c()) {
        Link_PollPeerReply();
    }
    func_01ff9bd0();
}
