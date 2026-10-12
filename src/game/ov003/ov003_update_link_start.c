#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"
#include "sdk/msl_string.h"

u16 Wireless_GetChildAids(void);
u16* Wireless_GetSharedData(u16 aid);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(void* a0);
void func_ov003_02363dbc(void);
extern char Ov003_StartMessage[];

/* Runs while the low mode nibble is 3. With the link flag Unk_02160ac0.unk_7c
 * set it scans the first 8 link slots for a received string that differs from
 * "cmstart" (Ov003_StartMessage): if one does, the link send buffer is set to
 * "cmstart", else it is cleared. Without a differing string it then calls
 * func_ov003_02363dbc. */
void Ov003_UpdateLinkStart(void) {
  int i;
  int found;

  if (data_ov003_02364928->phase != 3) {
    return;
  }
  found = 0;
  if (data_02160ac0->unk_7c != 0) {
    if (!func_0207bd94()) {
      return;
    }
    for (i = 0; i < 8; i++) {
      if ((Wireless_GetChildAids() | 1) & (1 << i)) {
        if (strcmp((const char*)Wireless_GetSharedData((u16)i),
                   Ov003_StartMessage)) {
          found = 1;
          break;
        }
      }
    }
    if (found == 1) {
      func_0207b4b0(Ov003_StartMessage);
    } else {
      func_0207b48c();
    }
  }
  if (found) {
    return;
  }
  func_ov003_02363dbc();
}
