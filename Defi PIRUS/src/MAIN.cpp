/*
============================================================================
 Name : MAIN.cpp
 Author :
 Version :0.81
 Description : Code Robus 1

 robot 47 P: 0,9 I:0,089
 robot 77 P: 1,70 I:0,220
============================================================================
*/

// Include Files
#include <libarmus.h>
#define PI 3.14159265358979323846264338327950288
#define LEFT_MOTOR 7
#define RIGHT_MOTOR 8
#define Circum 229.33626371205490640777296697940 //Cir de la roue
#define LEFT_ROT 1
#define RIGHT_ROT 0
#define SPEED_START 70
#define DELAY_STEP 500

float GAIN_I = 0.223;
float GAIN_P = 1.68;
int m_iTicTotalG = 0;
int m_iTicTotalD = 0;

int autre_test();


// Prototypes de fonctions de configs
void Initialisation();

// Prototypes de fonctions (Avancer, Tourner)
void Avance(int iDistance);
void Rotation(float iAngle, int iDirection);
float PID_Setup(void);

int main()
{
	
	//Configuration
	Initialisation();
	// variables locales
	int j = 0;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);
	
	while (j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(BMP_REAR))
		{
			LCD_Printf("Depart du programme");
			j = 1;
		}
	}
	
	//Demo a faire
	//Sonars
	//Couleurs
	//Infrarouge
	//Demo boutons
	int menu_option = 0;
	while(menu_option < 4)
	{
		LCD_Printf("Choisir la demonstration que vous voulez \n");
		LCD_Printf("1. Demonstration sonar\n");
		LCD_Printf("2. Demonstration detecteur de couleurs\n");
		LCD_Printf("3. Demonstration infrarouge\n");
		LCD_Printf("4. Sortie\n");
		
		THREAD_MSleep(3000);
		SYSTEM_ResetTimer();
		while (SYSTEM_ReadTimerMSeconds() < 5000)
		{
			THREAD_MSleep(100);
			if(DIGITALIO_Read(BMP_REAR))
			{
				menu_option++;
			}
		}
		
		switch(menu_option) {
			case 1 :
				LCD_Printf("1. Demonstration sonar\n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			case 2 : 
				LCD_Printf("2. Demonstration detecteur de couleurs\n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			case 3 : 
				LCD_Printf("3. Demonstration infrarouge\n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			default :
				LCD_Printf("4. Sortie\n");
				THREAD_MSleep(2000);
				if(autre_test() == 1)
				{
					menu_option = 0;
				}
				break;
		}
	}
	
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
		Avance(800);
		LCD_Printf("Tic coter Gauche: %d	Tic coter Droit: %d\n", m_iTicTotalG, m_iTicTotalD);
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

void Rotation(float fAngle, int iDirection)
{
	float fDroitSpeed = SPEED_START-20; //Define start speed
	float fGaucheSpeed = SPEED_START-20;
	float fArcRot = 0;
	float fTicToDo = 0;
	
	//Cacul des tics a faire (encodeurs)
	if (iDirection == RIGHT_ROT)
		fArcRot = ((PI * 141) * ((fAngle - 5) / 360));
	else
		fArcRot = ((PI * 141) * ((fAngle - 4) / 360));
	fTicToDo = (fArcRot / Circum) * 64;
	
	m_iTicTotalD = 0;
	m_iTicTotalG = 0;
	
	int iTicDone = m_iTicTotalG; // égual à 0 ou à m_TicTotalG...
	int iTicObjectif = iTicDone + fTicToDo; //Tic a avoir au total a la fin de la fonction

	//Remise a 0
	ENCODER_Read(2);
	ENCODER_Read(1);

	//Gauche
	if (iDirection == LEFT_ROT)
	{
		while((m_iTicTotalG < iTicObjectif) || ( m_iTicTotalG < iTicObjectif))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, (int)(-1*(fGaucheSpeed)));
			MOTOR_SetSpeed(RIGHT_MOTOR, (int)fDroitSpeed);
			fGaucheSpeed += PID_Setup();
		}
	}
	
	//Droite
	if (iDirection == RIGHT_ROT)
	{
		while((m_iTicTotalG < iTicObjectif) || ( m_iTicTotalG < iTicObjectif))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, (int)fGaucheSpeed);
			MOTOR_SetSpeed(RIGHT_MOTOR, (int)(-1*(fDroitSpeed)));
			fGaucheSpeed += PID_Setup();
		}
	}
	MOTOR_SetSpeed(LEFT_MOTOR, 0);
	MOTOR_SetSpeed(RIGHT_MOTOR, 0);
}

void Avance(int iDistance) //Distance en mm
{
	float fDroitSpeed = SPEED_START;
	float fGaucheSpeed = SPEED_START;
	float fTicToDo = 64 * (iDistance / Circum);//verifier calcul
	int iTicDone = m_iTicTotalG;
	
	int iTicObjectif = iTicDone + fTicToDo; //Tic a avoir a la fin de la fonction
	
	//Remise a 0 des encodeurs
	ENCODER_Read(2);
	ENCODER_Read(1);


	//Avance
	if (iDistance > 0)
	{
		while((m_iTicTotalG < iTicObjectif) || (m_iTicTotalG < iTicObjectif))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, fGaucheSpeed);
			MOTOR_SetSpeed(RIGHT_MOTOR, fDroitSpeed);
			fGaucheSpeed += PID_Setup();
		}
	}
	//Recule
	if (iDistance < 0)
	{
		while((m_iTicTotalG < iTicObjectif) || ( m_iTicTotalG < iTicObjectif))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, -1*(fGaucheSpeed));
			MOTOR_SetSpeed(RIGHT_MOTOR, -1*(fDroitSpeed));
			fGaucheSpeed += PID_Setup();
		}
	}
	//Arreter les moteurs
	MOTOR_SetSpeed(LEFT_MOTOR, 0);
	MOTOR_SetSpeed(RIGHT_MOTOR, 0);
}

int autre_test()
{
	LCD_ClearAndPrint("Voulez vous faire une autre demo?\n Si oui, tenir le bumber arriere.\n");
	THREAD_MSleep(500);
	if(DIGITALIO_Read(BMP_REAR))
	{
			return 1;
	}
	return 0;
	
}


