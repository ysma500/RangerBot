#include <libarmus.h>
#include "speaker.h"
#include "boutons.h"

int play_siren()
{
	int alarm = 0;
	AUDIO_SetVolume(100);
	while (alarm == 0);
	{
		AUDIO_PlayFile("Siren.wav");
		if (DIGITALIO_Read(ORANGE_LEFT))
		{
			alarm = 1;
		}
	}
	return 0;
}

int play_setup(void)
{
  AUDIO_SetVolume(50);
  
  int test = 0;
  char A[] = "BOMB.wav";
  char B[] = "Tornado.wav";
  char C[] = "Siren.wav";
  while (test == 0)
  {
	Play(C);
	if(DIGITALIO_Read(ORANGE_LEFT))
	{
		test = 1;
	}
  }
  return 0;
}

void Play(char music[])
{
	int play = 0;
	int current = 0;
	current = AUDIO_PlayFile(music);
	LCD_ClearAndPrint("Lecture de %c \nAppuyer sur le bouton orange a gauche pour arreter", music[0]);
	while (play == 0)
	{
		if (DIGITALIO_Read(ORANGE_LEFT))
		{
		  play = 1;
		  break;
		}
		THREAD_MSleep(100);
	}
  AUDIO_StopPlayback(current);
}

