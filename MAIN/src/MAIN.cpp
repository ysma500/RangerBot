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
void servo_turn();

int main()
{
	// variables locales
	int i;

	// d�clarations des threads
	THREAD thread_bumpers;
	THREAD thread_servos;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");

	// d�part des threads (voir les d�finitions des fonctions plus bas)
	thread_bumpers = THREAD_CreateSimple(bumper_watch);
	thread_servos  = THREAD_CreateSimple(servo_turn);

	// d�part des moteurs en acc�l�ration
	for (i=2; i<11; i++)
	{
		MOTOR_SetSpeed(MOTOR_LEFT,  i*10);
		MOTOR_SetSpeed(MOTOR_RIGHT, i*10);
		THREAD_MSleep (1000);
	}
	LCD_Printf("Le robot a atteint sa pleine vitesse\n");

	AUDIO_SetVolume(50);

	// Vous devez ins�rez un fichier audio avec ce nom sur votre cl� usb
	//		dans le r�pertoire armus afin que cela fonctionne
	//AUDIO_PlayMP3File("songno1.mp3");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On d�truit les threads
	THREAD_Destroy(&thread_bumpers);
	THREAD_Destroy(&thread_servos);

	// On arrete tout sur la carte d'entr�es/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}


// Les threads suivant sont ex�cut�s en parall�le du main() � partir du moment ou ils sont cr��s
// 		avec la fonction THREAD_CreateSimple()

void bumper_watch()
{
	while(1)
	{
		// si la "bumper switch" avant de robus est enclanch�e...
		if(DIGITALIO_Read(BMP_FRONT))
		{
			// part les moteurs � reculons
			MOTOR_SetSpeed(MOTOR_LEFT,  -50);
			MOTOR_SetSpeed(MOTOR_RIGHT, -50);
			// attend 2 secondes
			THREAD_MSleep(2000);
			// part le moteur gauche vers l'avant (pour que le robot tourne sur lui-m�me)
			MOTOR_SetSpeed(MOTOR_LEFT, 50);
			// attend 300 millisecondes
			THREAD_MSleep(300);
		}
		// attend 50 millisecondes
		THREAD_MSleep(50);
	}
}

void servo_turn()
{
	while(1)
	{
		// Positionne les servomoteurs � 45 degr�s
		SERVO_SetAngle(9,  45);
		SERVO_SetAngle(10, 45);

		// Attend 2,5 secondes
		THREAD_MSleep(2500);

		// Positionne les servomoteurs � 135 degr�s
		SERVO_SetAngle(9,  135);
		SERVO_SetAngle(10, 135);

		// Attend 5 secondes
		THREAD_MSleep(5000);
	}
}




