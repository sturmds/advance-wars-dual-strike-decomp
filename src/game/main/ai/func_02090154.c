#include "types.h"
#include "game/ai_build_state.h"

int func_0209f980(int arg);
int func_02091538(void);
int AiTurn_TryBuildLander(void);
int func_020908bc(void);
int func_020907b8(void);
int func_020913e8(void);

/* End-actions step 8. Tries five sub-steps in order and returns 1 at the first
 * one that returns 1 (needs another frame). Their purposes are not known. */
BOOL func_02090154(void)
{
    data_021619c0->unk_08 = func_0209f980(1);
    if (func_02091538() == 1) {
        return 1;
    }
    if (AiTurn_TryBuildLander() == 1) {
        return 1;
    }
    if (func_020908bc() == 1) {
        return 1;
    }
    if (func_020907b8() == 1) {
        return 1;
    }
    return func_020913e8() == 1;
}
