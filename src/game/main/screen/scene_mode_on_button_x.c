#include "types.h"

void Sound_PlaySe(int id);
int SceneMode_IsChanging(void);
int SceneMode_RequestNext(void);

/* Map X button: unless a mode change is already running, requests the next
 * mode and plays sound effect 155. Returns TRUE if it requested one. */
int SceneMode_OnButtonX(void) {
  if (!SceneMode_IsChanging() && SceneMode_RequestNext()) {
    Sound_PlaySe(155);
    return 1;
  }
  return 0;
}
