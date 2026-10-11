#include "types.h"
#include "game/Unk_021694c0.h"

int func_020f35cc(void);
int func_020f4658(int mode);

extern Unk_021694c0 *data_021694c0;
extern int *data_021695a4[];

/* With no change pending, looks up the current mode in the transition list
 * of this scene kind (data_021694c0 unk_00) and requests the mode that
 * follows it in the list. The list ends with -1. Returns 0 if there is none. */
int SceneMode_RequestNext(void)
{
    int i;
    int *transitions;

    if (data_021694c0->unk_0c != data_021694c0->unk_08) {
        return 0;
    }
    transitions = data_021695a4[func_020f35cc()];
    for (i = 0; transitions[i] != -1; i++) {
        if (data_021694c0->unk_0c == transitions[i]) {
            return func_020f4658(transitions[i + 1]);
        }
    }
    return 0;
}
