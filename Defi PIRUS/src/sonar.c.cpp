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
#define SONAR_UN 1
#define SONAR_DEUX 2

float GAIN_I = 0.223;
float GAIN_P = 1.68;
int m_iTicTotalG = 0;
int m_iTicTotalD = 0;

// Prototypes de fonctions de configs
void Initialisation();

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

	LCD_Printf("Le robot a termine le parcours\n");

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}

//Debut de la fonction pour la modification des gains a suivre 
void Initialisation()
{
	int i = 0;
	float fSonarUn = 0, fSonarDeux = 0;
	THREAD_MSleep(5000);
	while(i==0)
	{
		fSonarUn = SONAR_Detect(SONAR_UN);
		fSonarDeux = SONAR_Detect(SONAR_DEUX);
		LCD_Printf("1: %0.4fcm 2: %0.4fcm \n", fSonarUn, fSonarDeux);

		if(DIGITALIO_Read(BMP_REAR))
		{
			i = 1;
			LCD_Printf("Sortie des configs\n");
		}
		THREAD_MSleep(250);
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




