/*
============================================================================
 Name : MAIN.cpp
 Author :
 Version :0.81
 Description : Code Robus 1

 robot 47 P: I:
 robot 77 P: I:
============================================================================
*/

// Include Files
#include <libarmus.h>
#define PI 3.14159265358979323846264338327950288
#define LEFT_MOTOR 7
#define RIGHT_MOTOR 8

float GAIN_I = 0.089;
float GAIN_P = 0.90;
int m_iTicTotalG = 0;
int m_iTicTotalD = 0;

// Prototypes de fonctions de configs
void Initialisation();

// Prototypes de fonctions (Avancer, Tourner)
void avancer_distance(int iDistance);
void rotation_angle(float iAngle);
float PID_Setup(void);

int main()
{
	// variables locales
	int i;
	int j = 0;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");
	
	while (j == 0)
	{
		if(DIGITALIO_Read(BMP_REAR))
		{
			j = 1;
		}
		THREAD_MSleep(1000);
	}
	
	//Configuration
	Initialisation();
	
	
	j = 0;
	while (j == 0)
	{
		if(DIGITALIO_Read(BMP_REAR))
		{
			j = 1;
			LCD_Printf(" Départ dans 5");
		}
		THREAD_MSleep(5000);

	}
	// Depart du circuit
	avancer_distance(2210);

	rotation_angle(90.0);

	avancer_distance(430);

	rotation_angle(-90.0);

	avancer_distance(450);

	rotation_angle(-90.0);

	avancer_distance(430);

	rotation_angle(90.0);

	avancer_distance(400);

	rotation_angle(-45.0);

	avancer_distance(560);

	rotation_angle(90.0);

	avancer_distance(820);

	rotation_angle(-45.0);

	avancer_distance(500);

	rotation_angle(-12.5);

	avancer_distance(460);

	rotation_angle(180.0);

	AUDIO_SetVolume(50);
	AUDIO_PlayFile("thug.wav");
	THREAD_MSleep(10000);

	avancer_distance(-460);

	LCD_Printf("Le robot a termine le parcours\n");


	// Vous devez inserez un fichier audio avec ce nom sur votre cle usb
	//		dans le repertoire armus afin que cela fonctionne
	//AUDIO_PlayMP3File("songno1.mp3");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}

//Debut de la fonction pour la modification des gains a suivre 
void Initialisation()
{
	int i = 0, j = 0;

	while(i==0 || j==0)
	{
		avancer_distance (500);
		//Si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT) && DIGITALIO_Read(BMP_LEFT))
		{
			LCD_Printf("Wait 1 sec\n");
			//Attends 1000 millisecondes
			THREAD_MSleep(1000);
			int k = 0;
			LCD_Printf("ajustement de GAIN_P\n");
			while(k == 0)
			{
				if(DIGITALIO_Read(BMP_LEFT))
				{
					GAIN_P += 0.01;
					LCD_Printf("Augmentation de 0,01 = %f\n", GAIN_P);
				}
				if(DIGITALIO_Read(BMP_RIGHT))
				{
					GAIN_P -= 0.01;
					LCD_Printf("Diminution de 0,01 = %f\n", GAIN_P);
				}
				if(DIGITALIO_Read(BMP_FRONT))
				{
					LCD_Printf("Fermeture des modification de GAIN_P\n");
					k = 1;
					m_iTicTotalG = 0;
					m_iTicTotalD = 0;
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
				//avancer_distance (2000);
				if(DIGITALIO_Read(BMP_LEFT))
				{
					GAIN_I += 0.001;
					LCD_Printf("Augmentation de 0,001 = %f\n", GAIN_I);
				}
				if(DIGITALIO_Read(BMP_RIGHT))
				{
					GAIN_I -= 0.001;
					LCD_Printf("Diminution de 0,001 = %f\n", GAIN_I);
				}
				if(DIGITALIO_Read(BMP_FRONT))
				{
					LCD_Printf("Fermeture des modification de GAIN_I\n");
					k=1;
					m_iTicTotalG = 0;
					m_iTicTotalD = 0;
				}
				// attend 50 millisecondes
				THREAD_MSleep(50);
			}
		}
		if(DIGITALIO_Read(BMP_REAR))
		{
			i = 1;
			j = 1;
			LCD_Printf("Sortie des configs\n");
		}
	}
	MOTOR_SetSpeed(7, 0);
	MOTOR_SetSpeed(8, 0);
}


float PID_Setup()
{
	int iCorrP = 0, iCorrI = 0, iTicGRead = 0, iTicDRead = 0, iErrorLive = 0, iErrorTotal = 0;
	
	THREAD_MSleep(50);
	iTicDRead = ENCODER_Read(2);
	iTicGRead = ENCODER_Read(1);
	
	m_iTicTotalD += iTicDRead;
	m_iTicTotalG += iTicGRead;
	iErrorLive = iTicDRead - iTicGRead;
	iErrorTotal = m_iTicTotalD - m_iTicTotalG;
	
	iCorrP = GAIN_P * iErrorLive;
	iCorrI = GAIN_I * iErrorTotal;
	
	return (iCorrP + iCorrI);
}

void rotation_angle(float fAngle)
{
	float fDroitSpeed = 50;
	float fGaucheSpeed = 50;
	float fArcLength = ((2 * PI * 140) * (fAngle / 360));
	float fTicToDo = (fArcLength / (2 * PI * 36.5)) * 64;
	
	//Remise a 0
	ENCODER_Read(2);
	ENCODER_Read(1);
	
	//Gauche
	if (fAngle > 0)
	{
		while(m_iTicTotalD < (iTicDone + fTicToDo))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, -1*(fGaucheSpeed));
			MOTOR_SetSpeed(RIGHT_MOTOR, fDroitSpeed);
			fGaucheSpeed += PID_Setup();
		}
	}
	
	//Droit
	if (fAngle < 0)
	{
		while(m_iTicTotalD < (iTicDone - fTicToDo))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, fGaucheSpeed);
			MOTOR_SetSpeed(RIGHT_MOTOR, -1*(fDroitSpeed));
			fGaucheSpeed += PID_Setup();
		}
	}
}


void avancer_distance(int iDistance)
{
	float fDroitSpeed = 50;
	float fGaucheSpeed = 50;
	float fTicToDo = (iDistance / (2 * PI * 36.5)) * 64;
	int iTicDone = m_iTicTotalD;
	
	//Remise a 0
	ENCODER_Read(2);
	ENCODER_Read(1);
	
	//Avance
	if (iDistance > 0)
	{
		while(m_iTicTotalD < (iTicDone + fTicToDo))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, fGaucheSpeed);
			MOTOR_SetSpeed(RIGHT_MOTOR, fDroitSpeed);
			fGaucheSpeed += PID_Setup();
		}
	}
	
	//Recule
	if (iDistance < 0)
	{
		while(m_iTicTotalD < (iTicDone - fTicToDo))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, -1*(fGaucheSpeed));
			MOTOR_SetSpeed(RIGHT_MOTOR, -1*(fDroitSpeed));
			fGaucheSpeed += PID_Setup();
		}
	}
}




