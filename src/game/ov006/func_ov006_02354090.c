#include "types.h"
#include "game/event_script.h"
#include "game/Unk_ov006_0235f690.h"

void func_0201e954(void);
void Gfx_ClearLayerMap(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Ov006_PostRequest(int request);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern EventScriptCommand data_ov006_0236b01c[];
extern EventScriptCommand data_ov006_0236b0bc[];

void func_ov006_02354090(void) {
  func_0201e954();
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  Ov006_PostRequest(7);
  EventScript_Start((data_ov006_0236d640->unk_00 & 0x1000)
                        ? data_ov006_0236b0bc
                        : data_ov006_0236b01c);
}
