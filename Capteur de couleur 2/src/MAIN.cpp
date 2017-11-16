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

// Include Files armus
#include <libarmus.h>
//define
#define PI 3.14159265358979323846264338327950288
#define LEFT_MOTOR 7
#define RIGHT_MOTOR 8
#define Circum 229.33626371205490640777296697940 //Cir de la roue
#define LEFT_ROT 1
#define RIGHT_ROT 0
#define SPEED_START 70
#define DELAY_STEP 500
#define MIN_DISTANCE 200

//Define du capteur de couleurs
#define ADJD_S371_QR999_SADR 	0x74
#define CAP_RED					0x6
#define CAP_GREEN				0x7
#define CAP_BLUE				0x8
#define CAP_CLEAR				0x9
#define INTEGRATION_RED			10
#define INTEGRATION_GREEN		12
#define INTEGRATION_BLUE		14
#define INTEGRATION_CLEAR		16
#define ADJD_REG_CTRL			0
#define ADJD_REG_CONFIG			1
#define DATA_RED_LO				64
#define DATA_GREEN_LO			66
#define DATA_BLUE_LO			68
#define DATA_CLEAR_LO			70

#define CTRL_GSSR				0
#define CTRL_GOFS				1

#define CONFIG_TOFS				0

//Couleurs des coins : Rose, vert, bleu, jaune, Centre : gris, contour : rouge
//Define des couleurs pour le capteurs de couleurs
#define RED_R 544
#define RED_G 266
#define RED_B 184
#define RED_C 887

#define GREY_R 560
#define GREY_G 632
#define GREY_B 510
#define GREY_C 1024

#define YELLOW_R 955
#define YELLOW_G 917
#define YELLOW_B 305
#define YELLOW_C 1016

#define PINK_R 780
#define PINK_G 475
#define PINK_B 390
#define PINK_C 1015

#define GREEN_R 212
#define GREEN_G 305
#define GREEN_B 218
#define GREEN_C 712

#define BLUE_R 208
#define BLUE_G 240
#define BLUE_B 355
#define BLUE_C 758

#define WHITE_R 1020
#define WHITE_G 1018
#define WHITE_B 905
#define WHITE_C 1023

//Code de chaque couleur
#define START_RED 1
#define START_GREY 2
#define START_YELLOW 3
#define START_PINK 4
#define START_GREEN 5
#define START_BLUE 6
#define START_WHITE 7
#define START_OTHER 0

#define LINE_HYST 50 //Incertitude sur la detection de couleurs
#define LINE_HYST_PLUS 100 //incertitude sur jaune et rouge

//PID
float GAIN_I = 0.223;
float GAIN_P = 1.68;
int m_iTicTotalG = 0;
int m_iTicTotalD = 0;

//Valeur de couleur de depart
int m_iCouleurDep = 0;
//Fonction pour savoir la couleur au moment
int get_current_color();

// Prototypes de fonctions de configs
void Initialisation();

//Prototypes de fonctions capteur de couleur, pas besoin de comprendre
void adjd_SetRegister(unsigned char reg, unsigned char val);
void adjd_SetRegister16(unsigned char reg, int val);
unsigned char adjd_ReadRegister(unsigned char reg);
int adjd_ReadRegister16(unsigned char reg);
unsigned char cap_GetValue(unsigned char cap_address);
void cap_SetValue(unsigned char cap_address, unsigned char cap_value);
int integrationTime_GetValue(unsigned char address);
void integrationTime_SetValue(unsigned char address, int time_value);
void led_TurnOff();	//Controle sortie digitale 9
void led_TurnOn();	//Controle sortie digitale 9
void color_Read(int& data_red, int& data_blue, int& data_green, int& data_clear);
void color_ReadToCalibrate(int& data_red, int& data_blue, int& data_green, int& data_clear);
int color_Init(int& dev_handle);

int adjd_dev;

// Prototypes de fonctions (Avancer, Tourner)
void Avance(int iDistance, int iSens);
void Rotation(float iAngle, int iDirection);
float PID_Setup(void);
void Tourne_gauche_avance();
void Tourne_gauche_avance(int valeurMod);
void Avance_BASE();

int main()
{
	//Initialisation du capteur de couleur
	ERROR_CHECK(color_Init(adjd_dev));

	cap_SetValue(CAP_RED, 15);
	cap_SetValue(CAP_GREEN, 15);
	cap_SetValue(CAP_BLUE, 15);
	cap_SetValue(CAP_CLEAR, 15);

	integrationTime_SetValue(INTEGRATION_RED, 255);
	integrationTime_SetValue(INTEGRATION_GREEN, 255);
	integrationTime_SetValue(INTEGRATION_BLUE, 255);
	integrationTime_SetValue(INTEGRATION_CLEAR, 255);

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");

	//Configuration
	//Initialisation();

	int j = 0; //Debut du programme
	while (j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(BMP_REAR))
		{
			LCD_Printf("Debut de l'ecoute du 5kHz\n");
			j = 1;
		}
	}

	//Strategie pour le parcours
	float f_time = 0; //Initialisation compteur temps de la course
	float f_MSdepuis = 0; //millisecondes depuis la derniere lecture
	SYSTEM_ResetTimer(); //Mise a zero du compteur de la course
	int current_color;

	while (f_time <= TOTAL_TIME)
	{
		current_color = get_current_color();

		if (current_color == START_RED)
		{
			//LCD_Printf("RED\n");
			Tourne_gauche_avance();
		}
		else if(current_color == m_iCouleurDep)
		{
			//LCD_Printf("COULEUR DE DEPART \n");
			//Stop, backup and 180 degree turn
		}
		else
		{
			switch(current_color)
			{
				case START_GREY:
					//LCD_Printf("GREY \n");
					//if (INFRAROUGE)Si les infrarouges voient la boite a moins de 20cm, tourne a droite
					/*Sinon, avance...else
					{
						Avance_BASE();
					}*/
					Avance_BASE();
					break;
				case START_YELLOW:
					//LCD_Printf("YELLOW \n");
					Tourne_gauche_avance();
					break;
				case START_PINK:
					//LCD_Printf("PINK \n");
					Tourne_gauche_avance();
					break;
				case START_GREEN:
					//LCD_Printf("GREEN \n");
					Avance(-MIN_DISTANCE);
					Tourne_gauche_avance();
					break;
				case START_BLUE:
					//LCD_Printf("BLUE \n");
					//Si tu as un objet dans les pinces, tourne a droite et avance
					//Sinon tourne a gauche et avance
					Tourne_gauche_avance();
					break;
				case START_WHITE:
					//LCD_Printf("WHITE \n");
					Avance_BASE();
					break;
				default:
					//LCD_Printf("I don't know where the fuck I am\n");
					Avance_BASE();
					//Code de detection de ligne?
					break;
			}
		}

		THREAD_MSleep(10);
		f_MSdepuis = SYSTEM_ReadTimerMSeconds();
		f_time = f_time + f_MSdepuis;
		SYSTEM_ResetTimer();
	}

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}
//Get current color function
int get_current_color()
{
	int color;
	int red, blue, green, clear;
	color_Read(red, blue, green, clear);
			//Red
			if (red >= (RED_R - LINE_HYST_PLUS) && red <= (RED_R + LINE_HYST_PLUS)
				&& blue >= (RED_B - LINE_HYST_PLUS) && blue <= (RED_B + LINE_HYST_PLUS)
				&& green >= (RED_G - LINE_HYST_PLUS) && green <= (RED_G + LINE_HYST_PLUS)
				&& clear >= (RED_C - LINE_HYST_PLUS) && clear <= (RED_C + LINE_HYST_PLUS))
			{
				color = START_RED;
			}
			//Grey
			else if (red >= (GREY_R - LINE_HYST) && red <= (GREY_R + LINE_HYST)
				&& blue >= (GREY_B - LINE_HYST) && blue <= (GREY_B + LINE_HYST)
				&& green >= (GREY_G - LINE_HYST) && green <= (GREY_G + LINE_HYST)
				&& clear >= (GREY_C - LINE_HYST) && clear <= (GREY_C + LINE_HYST))
			{
				color = START_GREY;
			}
			//Yellow
			else if (red >= (YELLOW_R - LINE_HYST_PLUS) && red <= (YELLOW_R + LINE_HYST_PLUS)
				&& blue >= (YELLOW_B - LINE_HYST_PLUS) && blue <= (YELLOW_B + LINE_HYST_PLUS)
				&& green >= (YELLOW_G - LINE_HYST_PLUS) && green <= (YELLOW_G + LINE_HYST_PLUS)
				&& clear >= (YELLOW_C - LINE_HYST_PLUS) && clear <= (YELLOW_C + LINE_HYST_PLUS))
			{
				color = START_YELLOW;
			}
			//Pink
			else if (red >= (PINK_R - LINE_HYST) && red <= (PINK_R + LINE_HYST)
				&& blue >= (PINK_B - LINE_HYST) && blue <= (PINK_B + LINE_HYST)
				&& green >= (PINK_G - LINE_HYST) && green <= (PINK_G + LINE_HYST)
				&& clear >= (PINK_C - LINE_HYST) && clear <= (PINK_C + LINE_HYST))
			{
				color = START_PINK;
			}
			//Green
			else if (red >= (GREEN_R - LINE_HYST) && red <= (GREEN_R + LINE_HYST)
				&& blue >= (GREEN_B - LINE_HYST) && blue <= (GREEN_B + LINE_HYST)
				&& green >= (GREEN_G - LINE_HYST) && green <= (GREEN_G + LINE_HYST)
				&& clear >= (GREEN_C - LINE_HYST) && clear <= (GREEN_C + LINE_HYST))
			{
				color = START_GREEN;
			}
			//Blue
			else if (red >= (BLUE_R - LINE_HYST) && red <= (BLUE_R + LINE_HYST)
				&& blue >= (BLUE_B - LINE_HYST) && blue <= (BLUE_B + LINE_HYST)
				&& green >= (BLUE_G - LINE_HYST) && green <= (BLUE_G + LINE_HYST)
				&& clear >= (BLUE_C - LINE_HYST) && clear <= (BLUE_C + LINE_HYST))
			{
				color = START_BLUE;
			}
			//White
			else if (red >= (WHITE_R - LINE_HYST) && red <= (WHITE_R + LINE_HYST)
				&& blue >= (WHITE_B - LINE_HYST) && blue <= (WHITE_B + LINE_HYST)
				&& green >= (WHITE_G - LINE_HYST) && green <= (WHITE_G + LINE_HYST)
				&& clear >= (WHITE_C - LINE_HYST) && clear <= (WHITE_C + LINE_HYST))
			{
				color = START_WHITE;
			}
			else
			{
				color = START_OTHER;
			}
	return color;
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


	void Avance(int iDistance, int iSens) //Distance en mm
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
		MOTOR_SetSpeed(LEFT_MOTOR, 0);
		MOTOR_SetSpeed(RIGHT_MOTOR, 0);
	}

	///Fonction tourne ou avance

	void Tourne_gauche_avance()
	{
		Rotation(45,LEFT_ROT);
		Avance(MIN_DISTANCE, AVANCE);
	}

	void Tourne_gauche_avance(int valeurMod)
	{
		Rotation(45,LEFT_ROT);
		Avance(MIN_DISTANCE/3, AVANCE);
	}

	void Avance_BASE()
	{
		Avance(MIN_DISTANCE, AVANCE);
	}



///****************************************************************************
///*********** Fonctions pour le capteur de couleur ***************************
///****************************************************************************

//permet de changer la valeur des registres
void adjd_SetRegister(unsigned char reg, unsigned char val)
{
	unsigned char data[2];
	data[0] = reg;
	data[1] = val;
	armus::i2c_Write(adjd_dev, 2, data);
}

//permet de changer la valeur des registres de 16 bits
void adjd_SetRegister16(unsigned char reg, int val)
{
	unsigned char data[2];
	data[0] = reg;
	data[1] = val & 0xFF;
	armus::i2c_Write(adjd_dev, 2, data);
	data[0] = reg+1;
	data[1] = (val >> 8) & 0xFF;
	armus::i2c_Write(adjd_dev, 2, data);
}

//permet de lire la valeur des registres
unsigned char adjd_ReadRegister(unsigned char reg)
{
	unsigned char val;

	armus::i2c_ReadAfterWrite(adjd_dev, 1, &reg, 1, &val);

	return val;
}

//permet de lire la valeur des registres de 16 bits
int adjd_ReadRegister16(unsigned char reg)
{
	int val16;
	unsigned char val;
	armus::i2c_ReadAfterWrite(adjd_dev, 1, &reg, 1, &val);
	val16 = val;
	reg = reg+1;
	armus::i2c_ReadAfterWrite(adjd_dev, 1, &reg, 1, &val);
	val16 = val16 + ((val << 8) & 0xFF00);
	return val16;
}


// Permet de connaitre la valeur du CAP dans le registre
// prend comme argument une constante CAP_RED, CAP_BLUE, CAP_CLEAR ou CAP_GREEN
// retourne un unsigned char de la valeur
unsigned char cap_GetValue(unsigned char cap_address)
{
	unsigned char cap_value;

	cap_value = adjd_ReadRegister(cap_address);

	return cap_value;
}


// Permet de changer la valeur du CAP dans le registre
// prend comme premier argument une constante CAP_RED, CAP_BLUE, CAP_CLEAR ou CAP_GREEN
// le second argument est compris entre 0 et 15, et determine la valeur du cap a ecrire dans le registre
void cap_SetValue(unsigned char cap_address, unsigned char cap_value)
{
	adjd_SetRegister(cap_address, cap_value);
}



// Permet de connaitre la valeur du CAP dans le registre
// address est une constante comme INTEGRATION_RED, ...
// retourne un int de la valeur
int integrationTime_GetValue(unsigned char address)
{
	int time_value;

	time_value = adjd_ReadRegister16(address);

	return time_value;
}


// Permet de changer la valeur du CAP dans le registre
// address est une constante comme INTEGRATION_RED, ...
// time_value est compris entre 0 et 4095
void integrationTime_SetValue(unsigned char address, int time_value)
{
	adjd_SetRegister16(address, time_value);
}


// Vous devez vous-meme modifier cette fonction tout dependamment de la sortie numerique utilisee
void led_TurnOff()
{
	// TODO : code a changer
	DIGITALIO_Write(9, 0);
}

// Vous devez vous-meme modifier cette fonction tout dependamment de la sortie numerique utilisee
void led_TurnOn()
{
	// TODO : code a changer
	DIGITALIO_Write(9, 1);
}

// permet de faire une lecture differentielle avec et sans eclairage de la led
void color_Read(int& data_red, int& data_blue, int& data_green, int& data_clear)
{
	//premiere lecture sans eclairage
	led_TurnOff();

	adjd_SetRegister(ADJD_REG_CONFIG, 1 << CONFIG_TOFS);
	adjd_SetRegister(ADJD_REG_CTRL, 1 << CTRL_GOFS);
	while(adjd_ReadRegister(ADJD_REG_CTRL))
	{
		THREAD_MSleep(50);
	}

	//lecture avec eclairage
	led_TurnOn();
	adjd_SetRegister(ADJD_REG_CTRL, 1 << CTRL_GSSR);
	while(adjd_ReadRegister(ADJD_REG_CTRL))
	{
		THREAD_MSleep(50);
	}

	//eteindre la led
	led_TurnOff();

	data_red = adjd_ReadRegister16(DATA_RED_LO);
	data_green = adjd_ReadRegister16(DATA_GREEN_LO);
	data_blue = adjd_ReadRegister16(DATA_BLUE_LO);
	data_clear = adjd_ReadRegister16(DATA_CLEAR_LO);
}

void color_ReadToCalibrate(int& data_red, int& data_blue, int& data_green, int& data_clear)
{
	led_TurnOn();
	adjd_SetRegister(ADJD_REG_CONFIG, 0 << CONFIG_TOFS);
	adjd_SetRegister(ADJD_REG_CTRL, 1 << CTRL_GSSR);
	while(adjd_ReadRegister(ADJD_REG_CTRL))
	{
		THREAD_MSleep(50);
	}
	led_TurnOff();

	data_red = adjd_ReadRegister16(DATA_RED_LO);
	data_green = adjd_ReadRegister16(DATA_GREEN_LO);
	data_blue = adjd_ReadRegister16(DATA_BLUE_LO);
	data_clear = adjd_ReadRegister16(DATA_CLEAR_LO);

}

// l argument est un integer qui ne doit pas etre modifie
int color_Init(int& dev_handle)
{
	int error;
	error = armus::i2c_RegisterDevice(ADJD_S371_QR999_SADR, 100000, 1000, dev_handle);

	return error;
}

///****************************************************************************
///*********** FIN Fonctions pour le capteur de couleur ***********************
///****************************************************************************
