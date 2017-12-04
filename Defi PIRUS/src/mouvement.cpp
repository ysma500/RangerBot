// Include Files armus
#include <libarmus.h>
#include "mouvement.h"
#include "infrarouge.h"

float GAIN_I = 0.223;
float GAIN_P = 1.68;
int m_iTicTotalG = 0;
int m_iTicTotalD = 0;
int m_iFlagZoneBkp = 0;
int m_iFlagRge = 0;

//Debut de la fonction pour la modification des gains a suivre
void Initialisation()
{
	int i = 0, j = 0;

	while(i==0 || j==0)
	{
		Avance(800, AVANCE);
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

	int iTicDone = m_iTicTotalG; // �gual � 0 ou � m_TicTotalG...
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

void Avance(int iDistance, int iSens) //Distance en mm
{
	float fDroitSpeed = SPEED_START;
	float fGaucheSpeed = SPEED_START;
	float fTicToDo = 64 * (iDistance / Circum);
	int iTicDone = m_iTicTotalG;

	int iTicObjectif = iTicDone + fTicToDo; //Tic a avoir a la fin de la fonction

	//Remise a 0 des encodeurs
	ENCODER_Read(2);
	ENCODER_Read(1);


	//Avance
	if (iSens == AVANCE)
	{
		while((m_iTicTotalG < iTicObjectif) || (m_iTicTotalG < iTicObjectif))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, fGaucheSpeed);
			MOTOR_SetSpeed(RIGHT_MOTOR, fDroitSpeed);
			fGaucheSpeed += PID_Setup();
		}
	}
	//Recule
	if (iSens == RECULE)
	{
		while((m_iTicTotalG < iTicObjectif) || ( m_iTicTotalG < iTicObjectif))
		{
			MOTOR_SetSpeed(LEFT_MOTOR, -1*(fGaucheSpeed));
			MOTOR_SetSpeed(RIGHT_MOTOR, -1*(fDroitSpeed));
			fGaucheSpeed += PID_Setup();
		}
	}
	//Arreter les moteurs
	//J'ai commenter ceci par souci de fluiditer Y.F.
	//MOTOR_SetSpeed(LEFT_MOTOR, 0);
	//MOTOR_SetSpeed(RIGHT_MOTOR, 0);
}


/*
///Fonction combines
void Mouv_infra()
{
	lireCapteur(capteur_mov);
	if ((capteur_mov[DEVANT_MOV] < (capteur_mov[DROIT_MOV] - HYST_MOV)) && (capteur_mov[DEVANT_MOV] < (capteur_mov[GAUCHE_MOV] - HYST_MOV)))
	// si mur a droite et gauche mais pas devant avance tout droit
	{
		Avance(MIN_DISTANCE, AVANCE);
	}
	else if ((capteur_mov[DROIT_MOV] < (capteur_mov[GAUCHE_MOV] - HYST_MOV)))
	// si mur a gauche et devant mais pas a droite tourne a droite
	{
		Rotation(30,RIGHT_ROT);
		Avance(MIN_DISTANCE/2, AVANCE);
	}
	else if ((capteur_mov[GAUCHE_MOV] < (capteur_mov[DEVANT_MOV] - HYST_MOV)) && (capteur_mov[GAUCHE_MOV] < (capteur_mov[DROIT_MOV] - HYST_MOV)))
	// si mur a droite et tout droit mais pas a geuche tourne a gauche
	{
		Rotation(30,LEFT_ROT);
		Avance(MIN_DISTANCE/2, AVANCE);
	}
}
*/
