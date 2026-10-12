#include "types.h"

void FrameCallback_Remove(void (*callback)(void));
void MapPreview_CycleBorderColour(void);
void func_020d41a4(void);

void func_020d4124(void) {
  FrameCallback_Remove((void*)MapPreview_CycleBorderColour);
  func_020d41a4();
}
