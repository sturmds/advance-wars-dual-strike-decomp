#include "types.h"

void Sound_PlaySe(int id);
int func_020f48ec(void);
int SceneMode_RequestNext(void);

/* Map X button: unless a mode change is already running, requests the next
 * mode and plays sound effect 155. Returns TRUE if it requested one. */
int SceneMode_OnButtonX(void)
{
    if (!func_020f48ec() && SceneMode_RequestNext()) {
        Sound_PlaySe(155);
        return 1;
    }
    return 0;
}
