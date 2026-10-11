#include "types.h"
#include "game/Unk_021694c0.h"

int Scene_GetKind(void);
int SceneMode_Request(int mode);

extern Unk_021694c0 *data_021694c0;
extern int *data_021695a4[];

/* With no change pending, looks up the current mode in the transition list
 * of this scene kind (sceneKind) and requests the mode that
 * follows it in the list. The list ends with -1. Returns 0 if there is none. */
int SceneMode_RequestNext(void)
{
    int i;
    int *transitions;

    if (data_021694c0->requestedMode != data_021694c0->mode) {
        return 0;
    }
    transitions = data_021695a4[Scene_GetKind()];
    for (i = 0; transitions[i] != -1; i++) {
        if (data_021694c0->requestedMode == transitions[i]) {
            return SceneMode_Request(transitions[i + 1]);
        }
    }
    return 0;
}
