#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/campaign_recap.h"
#include "game/brightness_fade.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void MapScreen_UpdateAnimations(void);
u32 func_020ffa50(CampaignRecap* work, int index);
u32 func_020ffa3c(CampaignRecap* work, int index);
u32 func_020ffa28(CampaignRecap* work, int index);
BOOL CampaignRecap_IsEnd(CampaignRecap* work, int index);
void SpriteStrip_LoadMapName(int mapId, int arg);
void CampaignRecap_LoadResultSprites(CampaignRecap* work);
void func_020fed58(CampaignRecap* work, int mapOffset);
s32 HwMod32(s32 numerator, s32 denominator);
BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Screens_BlackOut(void);
void func_020ff07c(CampaignRecap* work);
Task* func_020fcb08(int arg);
BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
void Task_ResumeScript(CampaignRecapTask* task);
void CampaignRecap_DrawTitle(CampaignRecap* work);
void CampaignRecap_DrawPanels(CampaignRecap* work);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd data_0216a898[];

/* Per-frame update of the data_0216a8b0 task. It goes through the campaign
 * missions in work->unk_7c: for each it draws the map name on the sub
 * screen, loads the map on the main screen, fades in three sub screen
 * panels one after the other, waits 0x102 frames and fades them out. It
 * stops after 28 missions or at the first entry whose unk_04 is 0, fades
 * both screens and the song out, then resumes the task script. The name is
 * a guess: the code shows a walk through the missions, not when it plays. */
void CampaignRecap_Update(CampaignRecapTask* task) {
  CampaignRecap* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  CampaignRecapCameraOffsets xs;
  CampaignRecapCameraOffsets ys;
  int x;
  int y;

  MapScreen_UpdateAnimations();
  switch (task->step) {
    case 0:
      work->number = work->index + 1;
      work->mapOffset = func_020ffa50(work, work->index);
      work->unk_20 = func_020ffa3c(work, work->index);
      work->unk_30 = func_020ffa28(work, work->index);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      SpriteStrip_LoadMapName(work->mapOffset + 0xe0, 64);
      CampaignRecap_LoadResultSprites(work);
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      func_020fed58(work, work->mapOffset);
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      work->blend[0] = 0;
      work->blend[1] = 0;
      work->blend[2] = 0;
      task->frame = 0;
      if (work->firstStep == 3) {
        task->frames = 32;
      } else {
        task->frames = 1;
      }
      task->step++;
    case 1:
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        xs = data_0216a884;
        ys = data_0216a870;
        x = xs.v[HwMod32(work->index, CAMPAIGN_RECAP_CAMERA_OFFSETS)];
        y = ys.v[HwMod32(work->index, CAMPAIGN_RECAP_CAMERA_OFFSETS)];
        work->cameraStartX = x + (work->unk_44 * 50 - 800);
        work->cameraStartY = y + (-work->unk_48 * 50 + 600);
        work->cameraStartZ = -300;
        work->cameraEndX = work->unk_44 * 50 - 800;
        work->cameraEndY = -work->unk_48 * 50 + 600;
        work->cameraEndZ = 0;
        work->unk_4c = Task_Create(data_0216a898, TASK_LIST(3));
        task->step = work->firstStep;
      }
      break;
    case 3:
      work->fadeTask = func_020fcdc8(task, 24);
      work->fadeTask2 = func_020fcda0(task, 24);
      task->step++;
    case 4:
      if (!func_020fd624(work->fadeTask2)) {
        func_020fd618(work->fadeTask2);
        task->step = 10;
      }
      break;
    case 5:
      work->fadeTask = func_020fcdc8(task, 24);
      task->step = 10;
      break;
    case 10:
      task->frame = 0;
      task->frames = 24;
      task->step++;
    case 11:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      work->blend[0] = Math_Ease(0, 0, 16, task->frame, task->frames);
      work->slide[0] = Math_Ease(4, 24, 0, task->frame, task->frames);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = work->blend[0];
      data_027c0280->blendEvb = 16 - work->blend[0];
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step = 20;
      }
      break;
    case 20:
      task->frame = 0;
      task->frames = 24;
      task->step++;
    case 21:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      work->blend[1] = Math_Ease(0, 0, 16, task->frame, task->frames);
      work->slide[1] = Math_Ease(4, 24, 0, task->frame, task->frames);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = work->blend[1];
      data_027c0280->blendEvb = 16 - work->blend[1];
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step = 30;
      }
      break;
    case 30:
      task->frame = 0;
      task->frames = 24;
      task->step++;
    case 31:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      work->blend[2] = Math_Ease(0, 0, 16, task->frame, task->frames);
      work->slide[2] = Math_Ease(4, 24, 0, task->frame, task->frames);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = work->blend[2];
      data_027c0280->blendEvb = 16 - work->blend[2];
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        func_020fd618(work->fadeTask);
        task->step = 80;
      }
      break;
    case 80:
      task->frame = 0;
      task->frames = 0x102;
      task->step++;
    case 81:
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step = 90;
      }
      break;
    case 90:
      work->index++;
      if (CampaignRecap_IsEnd(work, work->index)) {
        task->step = 100;
        break;
      }
      BrightnessFade_StartOutBlackScreen0(task, 24);
      task->frame = 0;
      task->frames = 24;
      task->step++;
    case 91:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      work->blend[0] = Math_Ease(0, 16, 0, task->frame, task->frames);
      work->blend[1] = Math_Ease(0, 16, 0, task->frame, task->frames);
      work->blend[2] = Math_Ease(0, 16, 0, task->frame, task->frames);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = work->blend[0];
      data_027c0280->blendEvb = 16 - work->blend[0];
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step++;
      }
      break;
    case 92:
      if (!func_020fd624(work->fadeTask)) {
        func_020fd618(work->fadeTask);
        work->firstStep = 5;
        task->step = 0;
      }
      break;
    case 100:
      work->fadeTask = BrightnessFade_StartOutBlackScreen0(task, 64);
      work->fadeTask2 = func_020fcd50(task, 64);
      task->step++;
      break;
    case 101:
      if (func_020fd624(work->fadeTask)) {
        break;
      }
      if (func_020fd624(work->fadeTask2)) {
        break;
      }
      Screens_BlackOut();
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      func_020ff07c(work);
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      GFX_DISPCNT(data_027c0280)->bg0 = 0;
      GFX_DISPCNT(data_027c0280)->bg1 = 0;
      GFX_DISPCNT(data_027c0280)->bg2 = 0;
      GFX_DISPCNT(data_027c0280)->bg3 = 0;
      GFX_DISPCNT(data_027c0280)->obj = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      GFX_DISPCNT(data_027c0280)->bg0 = 0;
      GFX_DISPCNT(data_027c0280)->bg1 = 0;
      GFX_DISPCNT(data_027c0280)->bg2 = 0;
      GFX_DISPCNT(data_027c0280)->bg3 = 0;
      GFX_DISPCNT(data_027c0280)->obj = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      func_020fd618(work->fadeTask);
      func_020fd618(work->fadeTask2);
      task->step++;
      break;
    case 102:
      work->songFadeTask = func_020fcb08(1);
      task->step++;
    case 103:
      if (!BrightnessFade_IsActive(work->songFadeTask)) {
        func_020fccb0(work->songFadeTask);
        Task_ResumeScript(task);
      }
      break;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  CampaignRecap_DrawTitle(work);
  CampaignRecap_DrawPanels(work);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
