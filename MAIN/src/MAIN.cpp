/*
============================================================================
 Name : MAIN.cpp
 Author :
 Version :0.81
 Description : Code Robus 1
============================================================================
*/

// Include Files
#include <libarmus.h>
#define PI 3.1416

float GAIN_I = 0.05;
float GAIN_P = 0.5;

// Prototypes de fonctions de threads
void bumper_watch();
//void timer_watch();

// Prototypes de fonctions (Avancer, Tourner)
void avancer_distance(int iDistance);
void rotation_angle(float iAngle);
float PID_watch(int* iTicGauche,int* iTicGDroit);

int main()
{
	// variables locales
	int i;
	int j = 0;
	// declarations des threads
	//THREAD thread_bumpers;
	//THREAD thread_Timer;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");

	// depart des threads (voir les definitions des fonctions plus bas)
	bumper_watch();
	//thread_Timer = THREAD_CreateSimple(timer_watch);

	while (j == 0)
	{
		if(DIGITALIO_Read(BMP_REAR))
		{
			j = 1;
		}
		THREAD_MSleep(1000);

	}
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
	//THREAD_Destroy(&thread_bumpers);
	//THREAD_Destroy(&thread_Timer);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}


// Les threads suivant sont executes en parallele du main() a partir du moment ou ils sont crees
// 		avec la fonction THREAD_CreateSimple()

//Début de la fonction pour la modification des gain à suivre 
void bumper_watch()
{
	int i = 0, j = 0;

	while(i==0 || j==0)
	{
		avancer_distance (500);
		// si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT) && DIGITALIO_Read(BMP_LEFT))
		{
			LCD_Printf("Wait 1 sec\n");
			// attend 1000 millisecondes
			THREAD_MSleep(1000);
			int k = 0;
			LCD_Printf("ajustement de GAIN_P\n");
			while(k == 0)
			{
				if(DIGITALIO_Read(BMP_LEFT))
				{
					GAIN_P += 0.05;
					LCD_Printf("Augmentation de 0,05 = %f\n", GAIN_P);
				}
				if(DIGITALIO_Read(BMP_RIGHT))
				{
					GAIN_P -= 0.05;
					LCD_Printf("Diminution de 0,05 = %f\n", GAIN_P);
				}
				if(DIGITALIO_Read(BMP_FRONT))
				{
					LCD_Printf("Fermeture des modification de GAIN_P\n");
					j = 1;
					k=1;
				}
				// attend 50 millisecondes
				THREAD_MSleep(50);
			}

		}
		else if(DIGITALIO_Read(BMP_FRONT) && DIGITALIO_Read(BMP_RIGHT))
		{
			LCD_Printf("Wait 1 sec\n");
			// attend 1000 millisecondes
			THREAD_MSleep(1000);
			int k = 0;
			LCD_Printf("ajustement de GAIN_I\n");
			while(k == 0)
			{
				avancer_distance (2000);
				if(DIGITALIO_Read(BMP_LEFT))
				{
					GAIN_I += 0.001;
					LCD_Printf("Augmentation de 0,001 = %0.4f\n", GAIN_I);
				}
				if(DIGITALIO_Read(BMP_RIGHT))
				{
					GAIN_I -= 0.001;
					LCD_Printf("Diminution de 0,001 = %0.4f\n", GAIN_I);
				}
				if(DIGITALIO_Read(BMP_FRONT))
				{
					LCD_Printf("Fermeture des modification de GAIN_I\n");
					i = 1;
					k=1;
				}
				// attend 50 millisecondes
				THREAD_MSleep(50);
			}
		}
	}
	MOTOR_SetSpeed(7, 0);
	MOTOR_SetSpeed(8, 0);
}


/*void timer_watch()
{
	while (1)
	{
		//Temps
		int TIMER = TIMER + 50;
		THREAD_MSleep(50);
	}
}*/

float PID_watch(int* iTicGauche,int* iTicGDroit)
{
	int iCorrP = 0;
	int iCorrI = 0;
	int iClicGlive = 0;
	int iClicDlive = 0;
	int iVarClic = 0;
	int iIVarClic = 0;
	THREAD_MSleep(200);
	iClicDlive = ENCODER_Read(2);
	iClicGlive = ENCODER_Read(1);
	iVarClic = iClicDlive - iClicGlive;
	*iTicGDroit += iClicDlive;
	*iTicGauche += iClicGlive;
	iIVarClic = *iTicGDroit - *iTicGauche;
	iCorrP = GAIN_P * iVarClic;
	iCorrI = GAIN_I * iIVarClic;
	return (iCorrP + iCorrI);
}


//Fonction à Vérifier (integrer PID)
void rotation_angle(float fAngle)
{
	int iTicGauche;
	int iTicGDroit;
	float fDistance = 0;
	fDistance = fAngle*PI/180;

	float x = (fDistance / 7) + 1 ;
	while(iTicGauche < x && iTicGDroit < x)
	{
		int speed = 0;
		float fSpeed = 50;
		if (fDistance < 0)
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
	int iTicGauche = 0;
	int iTicGDroit = 0;
	//Remise a 0
	ENCODER_Read(2);
	ENCODER_Read(1);
	//Avance
	if (iDistance > 0)
	{
		float x = (iDistance / 7) + 1;
		while(iTicGauche < x && iTicGDroit < x )
		{
			MOTOR_SetSpeed(7, fGauche_speed);
			MOTOR_SetSpeed(8, fDroit_speed);
			fGauche_speed = fGauche_speed + PID_watch(&iTicGauche,&iTicGDroit);
		}
	}
	//Recule
	if (iDistance < 0)
	{
		float x = (iDistance / 7) - 1;
		while(iTicGauche < x && iTicGDroit < x )
		{
			MOTOR_SetSpeed(7, -(fGauche_speed));
			MOTOR_SetSpeed(8, -(fDroit_speed));
			fGauche_speed = fGauche_speed + PID_watch(&iTicGauche,&iTicGDroit);
		}
	}
}




