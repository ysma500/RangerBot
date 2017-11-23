#include <libarmus.h>

void Play(char music[]);

int main(void)
{
  AUDIO_SetVolume(50);
  LCD_ClearAndPrint("Départ du programme");
  int test = 0;
  char A[] = "BOMB.wav";
  char B[] = "Tornado.wav";
  char C[] = "Siren.wav";
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
    THREAD_MSleep(250);
  }
  return 0;
}

void Play(char music[])
{
  int play = 0;
  int current = 0;
  current = AUDIO_PlayFile(music);
  LCD_ClearAndPrint("Lecture de %c \nAppyyer sur 1 pour arreter", music[0]);
  while (play == 0)
  {
    if (DIGITALIO_Read(BMP_FRONT))
    {
      play = 1;
      break;
    }
    THREAD_MSleep(250);
  }
  AUDIO_StopPlayback(current);
}

