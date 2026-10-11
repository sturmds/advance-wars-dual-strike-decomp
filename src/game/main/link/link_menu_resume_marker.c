#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd gLinkMarkerScript0[];
extern const TaskScriptCmd gLinkMarkerScript1[];
extern const TaskScriptCmd gLinkMarkerScript2[];

void Task_ResumeByScript(const TaskScriptCmd *script);

/* Runs on past the wait in the script of the marker of this kind (0: stage 1, 1 and 2: the
 * picks of link players 0 and 1), which ends the marker task. */
void LinkMenu_ResumeMarker(int kind)
{
    switch (kind) {
    case 0:
        Task_ResumeByScript(gLinkMarkerScript0);
        break;
    case 1:
        Task_ResumeByScript(gLinkMarkerScript1);
        break;
    default:
        Task_ResumeByScript(gLinkMarkerScript2);
        break;
    }
}
