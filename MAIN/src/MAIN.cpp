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
void Avance(int iDistance);
void Rotation(float iAngle);
float PID_Setup(void);

int main()
{
	// variables locales
	int j = 0;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");
	
	while (j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(BMP_REAR))
		{
			LCD_Printf("Entrer dans les Configs\n");
			j = 1;
		}
	}
	
	//Configuration
	Initialisation();
	
	j = 0;
	while(j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(BMP_REAR))
		{
			LCD_Printf("Depart dans 5sec\n");
			THREAD_MSleep(5000);
			j = 1;
		}
	}
	// Depart du circuit
	Avance(2250);

	Rotation(90.0);

	Avance(475);

	Rotation(-90.0);

	Avance(450);

	Rotation(-90.0);

	Avance(475);	//Verifier les maths jusque ici (Ysmael)

	Rotation(90.0);

	Avance(400);

	Rotation(-45.0);

	Avance(560);

	Rotation(90.0);

	Avance(820);

	Rotation(-45.0);

	Avance(500);

	Rotation(-12.5);

	Avance(460);

	Rotation(180.0);

	AUDIO_SetVolume(50);
	AUDIO_PlayFile("thug.wav");
	THREAD_MSleep(10000);

	Avance(-460);

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
		Avance (500);
		//Si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT) && DIGITALIO_Read(BMP_LEFT))
		{
			LCD_Printf("Wait 1 sec\n");
			//Attends 1000 millisecondes
			THREAD_MSleep(1000);
			int k = 0;
			LCD_Printf("Ajustement de GAIN_P\n");
			while(k == 0)
			{
				if(DIGITALIO_Read(BMP_LEFT))
				{
					GAIN_P += 0.01;
					LCD_Printf("Augmentation de 0,01 = %0.2f\n", GAIN_P);
				}
				if(DIGITALIO_Read(BMP_RIGHT))
				{
					GAIN_P -= 0.01;
					LCD_Printf("Diminution de 0,01 = %0.2f\n", GAIN_P);
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
			LCD_Printf("Ajustement de GAIN_I\n");
			while(k == 0)
			{
				//Avance (2000);
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
	MOTOR_SetSpeed(LEFT_MOTOR, 0);
	MOTOR_SetSpeed(RIGHT_MOTOR, 0);
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

void Rotation(float fAngle)
{
	float fDroitSpeed = 50;
	float fGaucheSpeed = 50;
	float fArcLength = ((2 * PI * 140) * (fAngle / 360));
	float fTicToDo = (fArcLength / (PI * 36.5)) * 64;
	
	//Remise a 0
	ENCODER_Read(2);
	ENCODER_Read(1);
	int iTicDone = m_iTicTotalG; // égual à 0 ou à m_TicTotalG...
	//Gauche
	if (fAngle > 0)
	{
		while((m_iTicTotalG < (iTicDone + fTicToDo)) &&( m_iTicTotalG < (iTicDone + fTicToDo)))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, -1*(fGaucheSpeed));
			MOTOR_SetSpeed(RIGHT_MOTOR, fDroitSpeed);
			fGaucheSpeed += PID_Setup();
		}
	}
	
	//Droite
	if (fAngle < 0)
	{
		while((m_iTicTotalG < (iTicDone + fTicToDo)) &&( m_iTicTotalG < (iTicDone + fTicToDo)))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, fGaucheSpeed);
			MOTOR_SetSpeed(RIGHT_MOTOR, -1*(fDroitSpeed));
			fGaucheSpeed += PID_Setup();
		}
	}
}


void Avance(int iDistance)
{
	float fDroitSpeed = 50;
	float fGaucheSpeed = 50;
	float fTicToDo = (iDistance / (2 * PI * 36.5)) * 64;
	int iTicDone = m_iTicTotalG;
	
	//Remise a 0
	ENCODER_Read(2);
	ENCODER_Read(1);
	
	//Avance
	if (iDistance > 0)
	{
		while((m_iTicTotalG < (iTicDone + fTicToDo)) &&( m_iTicTotalG < (iTicDone + fTicToDo)))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, fGaucheSpeed);
			MOTOR_SetSpeed(RIGHT_MOTOR, fDroitSpeed);
			fGaucheSpeed += PID_Setup();
		}
	}
	
	//Recule
	if (iDistance < 0)
	{
		while((m_iTicTotalG < (iTicDone + fTicToDo)) &&( m_iTicTotalG < (iTicDone + fTicToDo)))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, -1*(fGaucheSpeed));
			MOTOR_SetSpeed(RIGHT_MOTOR, -1*(fDroitSpeed));
			fGaucheSpeed += PID_Setup();
		}
	}
}




