#include "sdk/snd.h"
#include "sdk/snd_seq.h"

int func_arm7_037ff818(SndExChannel* channel, int decay);
void func_arm7_037ff810(SndExChannel* channel, int sustain);
int func_arm7_037ff7f4(SndExChannel* channel, int release);

/* Starts the channel on the note's wave, square wave or noise and copies the note's
 * key, velocity, envelope and pan into it. FALSE when the wave is missing or the type unknown. */
BOOL SndArm7_NoteOn(SndExChannel* channel, int key, int velocity, s32 length,
                    SoundBank* bank, SoundBankNote* note) {
  SndWaveData* wave;
  BOOL started;

  switch (note->type) {
    case SOUND_NOTE_WAVE:
    case SOUND_NOTE_DIRECT_WAVE:
      if (note->type == SOUND_NOTE_WAVE) {
        wave = SndArm7_GetBankWaveData(bank, note->waveArcSlot, note->waveNo);
      } else {
        wave = (SndWaveData*)(note->waveArcSlot << 16 | note->waveNo);
      }
      if (wave == NULL) {
        started = FALSE;
      } else {
        started =
            SndExChannel_StartPcm(channel, &wave->param, wave->samples, length);
      }
      break;
    case SOUND_NOTE_PSG:
      started = SndExChannel_StartPsg(channel, note->waveNo, length);
      break;
    case SOUND_NOTE_NOISE:
      started = SndExChannel_StartNoise(channel, length);
      break;
    default:
      started = FALSE;
      break;
  }
  if (!started) {
    return FALSE;
  }
  channel->key = key;
  channel->originalKey = note->originalKey;
  channel->velocity = velocity;
  SndExChannel_SetAttack(channel, note->attack);
  func_arm7_037ff818(channel, note->decay);
  func_arm7_037ff810(channel, note->sustain);
  func_arm7_037ff7f4(channel, note->release);
  channel->initPan = note->pan - 64;
  return TRUE;
}
