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
#define PI 3.1416

// Prototypes de fonctions de threads
void bumper_watch();
void timer_watch();

// Prototypes de fonctions (Avancer, Tourner)
void avancer_distance(int iDistance);
void rotation_angle(float iAngle);
float PID_watch(int* iClicGauche,int* iClicDroit);

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
	avancer_distance(2220);

	rotation_angle(90);

	avancer_distance(430);

	rotation_angle(-90);

	avancer_distance(450);

	rotation_angle(-90);

	avancer_distance(430);

	rotation_angle(90);

	avancer_distance(400);

	rotation_angle(-45);

	avancer_distance(560);

	rotation_angle(90);

	avancer_distance(820);

	rotation_angle(-45);

	avancer_distance(500);

	rotation_angle(-12.5);

	avancer_distance(460);

	rotation_angle(180);

	AUDIO_SetVolume(50);
	AUDIO_PlayFile("thug.wav");
	THREAD_MSleep(10000);

	avancer_distance(-460);

	LCD_Printf("Le robot a termine le parcour\n");


	// Vous devez inserez un fichier audio avec ce nom sur votre cle usb
	//		dans le repertoire armus afin que cela fonctionne
	//AUDIO_PlayMP3File("songno1.mp3");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On detruit les threads
	THREAD_Destroy(&thread_bumpers);
	THREAD_Destroy(&thread_Timer);

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

float PID_watch(int* iClicGauche,int* iClicDroit)
{
	int iCorrP = 0;
	int iCorrI = 0;
	int iClicGlive = 0;
	int iClicDlive = 0;
	int iVarClic = 0;
	int iIVarClic = 0;
	int GAINP = 0.5;
	int GAINI = 0.5;
	THREAD_MSleep(200);
	iClicDlive = ENCODER_Read(2);
	iClicGlive = ENCODER_Read(1);
	iVarClic = iClicDlive - iClicGlive;
	*iClicDroit += iClicDlive;
	*iClicGauche += iClicGlive;
	iIVarClic = *iClicDroit - *iClicGauche;
	iCorrP = GAINP * iVarClic;
	iCorrI = GAINI * iIVarClic;
	return (iCorrP + iCorrI);
}


//Fonction à Vérifier (integrer PID)
void rotation_angle(float fAngle)
{
	int iClicGauche;
	int iClicDroit;
	float fDistance = 0;
	distance = angle*PI/180;

	float x = (distance / 7) + 1 ;
	while(iClicGauche < x && iClicDroit < x)
	{
		int speed = 0;
		float fSpeed = 50;
		if (distance < 0)
		{
			MOTOR_SetSpeed(7, fSpeed);
			MOTOR_SetSpeed(8, -fSpeed);
		}
		else
		{
			MOTOR_SetSpeed(7, -fSpeed);
			MOTOR_SetSpeed(8, fSpeed);
		}
	}
}


void avancer_distance(int iDistance)
{
	float fDroit_speed = 50;
	float fGauche_speed = 50;
	int iClicGauche = 0;
	int iClicDroit = 0;
	//Remise a 0
	ENCODER_Read(2);
	ENCODER_Read(1);
	//Avance
	if (iDistance > 0)
	{
		float x = (iDistance / 7) + 1;
		while(iClicGauche < x && iClicDroit < x )
		{
			MOTOR_SetSpeed(7, fGauche_speed);
			MOTOR_SetSpeed(8, fDroit_speed);
			fGauche_speed = fGauche_speed * PID_watch(&iClicGauche,&iClicDroit);
		}
	}
	//Recule
	if (iDistance < 0)
	{
		float x = (iDistance / 7) - 1;
		while(iClicGauche < x && iClicDroit < x )
		{
			MOTOR_SetSpeed(7, -(fGauche_speed));
			MOTOR_SetSpeed(8, -(fDroit_speed));
			fGauche_speed = fGauche_speed * PID_watch(&iClicGauche,&iClicDroit);
		}
	}
}




