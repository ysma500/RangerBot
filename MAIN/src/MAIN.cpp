/*
============================================================================
 Name : MAIN.cpp
 Author :
 Version :
 Description : Demo - Exe source file
============================================================================
*/

// Include Files
#include <libarmus.h>

// Prototypes de fonctions de threads
void bumper_watch();
void timer_watch();

// Prototypes de fonctions (Avancer, Tourner)
void avancer_distance(int clicGauche, int clicDroit, int distance);
void rotation_angle(float angle);
void PID_watch();

int main()
{
	// variables locales
	int i;

	// declarations des threads
	THREAD thread_bumpers;
	THREAD thread_Timer;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");

	// depart des threads (voir les definitions des fonctions plus bas)
	thread_bumpers = THREAD_CreateSimple(bumper_watch);
	thread_Timer = THREAD_CreateSimple(timer_watch);

	// Depart du circuit
	avancer_distance(iclicGauche, clicDroit, 2220);

	rotation_angle(90);

	avancer_distance(clicGauche, clicDroit, 430);

	rotation_angle(-90);

	avancer_distance(iclicGauche, clicDroit, 450);

	rotation_angle(-90);

	avancer_distance(clicGauche, clicDroit, 430);

	rotation_angle(90);

	avancer_distance(clicGauche, clicDroit, 400);

	rotation_angle(-45);

	avancer_distance(clicGauche, clicDroit, 560);

	rotation_angle(90);

	avancer_distance(clicGauche, clicDroit, 820);

	rotation_angle(-45);

	avancer_distance(clicGauche, clicDroit, 500);

	rotation_angle(-12.5);

	avancer_distance(clicGauche, clicDroit, 460);

	rotation_angle(180);

	AUDIO_SetVolume(50);
	//AUDIO_PlayMP3File("songno1.mp3");

	avancer_distance(clicGauche, clicDroit, 460);

	LCD_Printf("Le robot a atteint sa pleine vitesse\n");

	AUDIO_SetVolume(50);

	// Vous devez inserez un fichier audio avec ce nom sur votre cle usb
	//		dans le repertoire armus afin que cela fonctionne
	//AUDIO_PlayMP3File("songno1.mp3");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On detruit les threads
	THREAD_Destroy(&thread_bumpers);
	THREAD_Destroy(&thread_servos);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}


// Les threads suivant sont executes en parallele du main() a partir du moment ou ils sont crees
// 		avec la fonction THREAD_CreateSimple()

void bumper_watch()
{
	while(1)
	{
		// si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT))
		{
			// part les moteurs a reculons
			MOTOR_SetSpeed(MOTOR_LEFT,  -50);
			MOTOR_SetSpeed(MOTOR_RIGHT, -50);
			// attend 2 secondes
			THREAD_MSleep(2000);
			// part le moteur gauche vers l'avant (pour que le robot tourne sur lui-meme)
			MOTOR_SetSpeed(MOTOR_LEFT, 50);
			// attend 300 millisecondes
			THREAD_MSleep(300);
		}
		// attend 50 millisecondes
		THREAD_MSleep(50);
	}
}

void timer_watch()
{
	//...
}

void PID_watch()
{
	//...
}

void rotation_angle(float angle)
{
	//...
}

void avancer_distance(int clicGauche, int clicDroit, int distance);
{
		while(clicGauche && clicDroit < distance/7 + 1)
		{
			int speed = 0;
			speed = PID(50);
			MOTOR_SetSpeed(7, speed);
			MOTOR_SetSpeed(8, speed);
		}
}




