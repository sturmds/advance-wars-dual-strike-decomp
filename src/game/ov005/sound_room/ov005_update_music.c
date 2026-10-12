#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

void Sound_FadeMusicVolume(int from, int to, int frames);
void Sound_StopStream(int fadeFrames);
void Sound_StartMusicSeq(int seqNo);
void Sound_StartStream(int strmNo);

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* Task step that plays state->musicId. When the asked-for music differs
 * from what plays, it fades the old music out over 32 frames (a sequence
 * by volume, a stream with Sound_StopStream), waits for the fade, then
 * starts the new one, or nothing for -1. */
void Ov005_UpdateMusic(Ov005Task* task) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;

  switch (task->step) {
    case 0:
      if (state->musicKind != state->playingMusicKind ||
          state->musicId != state->playingMusicId) {
        task->step = 10;
      } else {
        state->playingMusicId = state->musicId;
        state->playingMusicKind = state->musicKind;
      }
      break;
    case 10:
      state->musicChanging = 1;
      if (state->playingMusicId != -1) {
        if (state->playingMusicKind == MUSIC_KIND_SEQ) {
          Sound_FadeMusicVolume(127, 0, 32);
        }
        if (state->playingMusicKind == MUSIC_KIND_STREAM) {
          Sound_StopStream(32);
        }
      }
      state->playingMusicId = state->musicId;
      state->playingMusicKind = state->musicKind;
      task->frame = 0;
      task->frames = 32;
      task->step++;
    case 11:
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame <= task->frames) {
        break;
      }
      state->musicChanging = 0;
      state->musicPlaying = 0;
      if (state->musicId != -1) {
        if (state->musicKind == MUSIC_KIND_SEQ) {
          task->step = 20;
        } else {
          task->step = 30;
        }
      } else {
        task->step = 0;
      }
      break;
    case 20:
      Sound_StartMusicSeq(state->musicId);
      state->musicPlaying = 1;
      task->step = 0;
      break;
    case 30:
      Sound_StartStream(state->musicId);
      state->musicPlaying = 1;
      task->step = 0;
      break;
  }
}
