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
void PID_watch();

// Prototypes de fonctions (Avancer, Tourner)
void avancer_distance(int clicGauche, int clicDroit, int distance);
void rotation_angle(float);

int main()
{
	// variables locales
	int i;

	// declarations des threads
	THREAD thread_bumpers;
	THREAD thread_PID;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");

	// depart des threads (voir les definitions des fonctions plus bas)
	thread_bumpers = THREAD_CreateSimple(bumper_watch);
	thread_PID = THREAD_CreateSimple(PID_watch);

	//Depart du circuit
	avancer_distance(int clicGauche, int clicDroit, int 2220);


	// depart des moteurs en acceleration
	for (i=2; i<11; i++)
	{
		MOTOR_SetSpeed(MOTOR_LEFT,  i*10 + PID);
		MOTOR_SetSpeed(MOTOR_RIGHT, i*10 + PID);
		THREAD_MSleep (1000);
	}
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

void PID_watch()
{
	//...
}




