#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/keypad.h"
#include "game/touch.h"

void Ov006_InitSlotButtons(void);
void Ov006_StartSlotsSlideIn(void);
void Ov006_ChooseButton36(void);
void Ov006_ChooseButton39(void);
int Ov006_DeleteMarkedUnit(void);
void Ov006_PaintTerrainAtCursor(void);
void Ov006_ScrollMapView(int step);
void Sound_PlaySe(int id);
void UnitShowTask_CreateAtTile(int x, int y, int unitType);
void func_020c4018(void);
void MapCursor_UpdateFree(void);
void Ov006_LoadTerrainPalettes(void);
void Ov006_ResetScreen1Effects(void);
void Ov006_MoveCursor(void);
void Ov006_UpdateCursorFromTouch(void);
void Ov006_CheckToolAtCursor(void);
void Ov006_PickToolAtCursor(int arg);
int Ov006_ReadEditInput(void);
int Ov006_PlaceUnitAtCursor(void);
void func_ov006_0235f690(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define STATE_50 (data_027c027c->unk_50[data_027c0284])
#define OV6 data_ov006_0236d640

static inline BOOL Touch_Is(int phase) {
  if (gTouch.active != 0 && gTouch.phase == phase) {
    return TRUE;
  }
  return FALSE;
}

/* One frame of the Design Room map editor. Flag 0x8000 of unk_00 sets the
 * editor up and puts the edit tile (cursorX, cursorY) on the map cursor. Each
 * frame moves the cursor and the view, then acts on Ov006_ReadEditInput:
 * 1 paints terrain (unitMode == 0) or places a unit, 2 deletes the unit on
 * the edit tile. Touch or B calls Ov006_PickToolAtCursor, R and L choose
 * buttons 39 and 36. Last it plays pendingSe, or else the cursor sound. */
void Ov006_UpdateMapEditor(void) {
  int result;
  int se;
  int action;

  if (OV6->unk_00 & 0x8000) {
    OV6->unk_00 &= ~0x8000;
    OV6->screenState = 0;
    OV6->cursorX = STATE->cursorX;
    OV6->cursorY = STATE->cursorY;
    Ov006_InitSlotButtons();
    Ov006_StartSlotsSlideIn();
    OV6->unk_00 |= 0x10;
    Ov006_LoadTerrainPalettes();
    OV6->unk_00 |= 4;
    func_ov006_0235f690();
    Ov006_ResetScreen1Effects();
  }
  if (OV6->screenState == 0) {
    OV6->screenState = 1;
    return;
  }
  if (OV6->recountDelay > 0) {
    OV6->recountDelay--;
    if (OV6->recountDelay <= 0) {
      OV6->recountDelay = 0;
      func_ov006_0235f690();
    }
  }

  result = 0;
  OV6->pendingSe = 0;
  if (gKeypad->held & PAD_BUTTON_X) {
    MapCursor_UpdateFree();
    if (OV6->cursorX != STATE->cursorX || OV6->cursorY != STATE->cursorY) {
      result = 1;
    }
  } else {
    Ov006_MoveCursor();
    Ov006_UpdateCursorFromTouch();
  }
  Ov006_ScrollMapView(4);
  if (OV6->cursorX != STATE->cursorX || OV6->cursorY != STATE->cursorY) {
    OV6->cursorX = STATE->cursorX;
    OV6->cursorY = STATE->cursorY;
  }
  Ov006_CheckToolAtCursor();
  if (OV6->cursorX == OV6->prevCursorX && OV6->cursorY == OV6->prevCursorY) {
    se = 0;
  } else if (result == 0) {
    se =
        0x10000; /* the cursor moved; the original leaves se unset when X moved it */
  }

  action = Ov006_ReadEditInput();
  if (action == 1) {
    if (OV6->unitMode == 0) {
      if (OV6->toolResult != 6) {
        Ov006_PaintTerrainAtCursor();
        if (STATE_50->first != NULL) {
          STATE_50->first();
        }
        func_020c4018();
        OV6->unk_00 |= 4;
      }
    } else {
      if (!(OV6->unk_00 & 0x40)) {
        result = Ov006_PlaceUnitAtCursor();
      } else {
        result = -1;
        if (Touch_Is(TOUCH_PRESSED) == TRUE && OV6->touchY > 32) {
          result = Ov006_PlaceUnitAtCursor();
        }
      }
      if (result == 1) {
        se = 0;
        OV6->pendingSe = 0;
        UnitShowTask_CreateAtTile(OV6->cursorX, OV6->cursorY,
                                  OV6->toolUnit & 0x3f);
      }
      if (result > 0) {
        OV6->unk_00 |= 0x1000;
        OV6->unk_00 |= 4;
      }
    }
    if (OV6->toolResult == 6 && (gKeypad->pressed & PAD_BUTTON_A)) {
      Sound_PlaySe(0x10008);
    }
  } else if (action == 2) {
    result = Ov006_DeleteMarkedUnit();
    if (result == 1) {
      se = 0;
      OV6->pendingSe = 0;
      UnitShowTask_CreateAtTile(OV6->cursorX, OV6->cursorY,
                                OV6->toolUnit & 0x3f);
    }
    if (result > 0) {
      OV6->unk_00 |= 0x1000;
      OV6->unk_00 |= 4;
    }
  }

  if (Touch_Is(TOUCH_PRESSED) == TRUE) {
    if (OV6->touchY > 32 && (OV6->unk_00 & 0x20)) {
      func_ov006_0235f690();
      Ov006_PickToolAtCursor(OV6->unitMode);
    }
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    func_ov006_0235f690();
    Ov006_PickToolAtCursor(OV6->unitMode);
  }
  if (gKeypad->pressed & (PAD_BUTTON_R | PAD_BUTTON_L)) {
    if (gKeypad->pressed & PAD_BUTTON_R) {
      Ov006_ChooseButton39();
    } else if (gKeypad->pressed & PAD_BUTTON_L) {
      Ov006_ChooseButton36();
    }
  }
  if (OV6->pendingSe != 0) {
    Sound_PlaySe(OV6->pendingSe);
    return;
  }
  if (se != 0) {
    Sound_PlaySe(se);
  }
}
