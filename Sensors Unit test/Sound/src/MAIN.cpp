#include <libarmus.h>

void Play(char[] music);

int main(void)
{
  AUDIO_SetVolume(50);

  char[] A = "BOMB.wav";
  char[] B = "Tornado.wav";
  char[] C = "Siren.wav";
  while (test == 0)
  {
    if (DIGITALIO_Read(BMP_FRONT))
    {
      Play(A);
    }
    else if (DIGITALIO_Read(BMP_RIGHT))
    {
      Play(B);
    }
    else if (DIGITALIO_Read(BMP_LEFT))
    {
      Play(C);
    }
  }
  return 0;
}

void Play(char[] music)
{
  int play = 0;
  int current = 0;
  current = AUDIO_PlayFile(music);
  LCD_ClearAndPrint("Lecture de %s \nAppyyer sur 1 pour arreter", music);
  while (play == 0)
  {
    if (DIGITALIO_Read(BMP_FRONT))
    {
      play = 1;
      THREAD_MSleep(250);
    }
  }
  AUDIO_StopPlayback(current);
  return;
}

