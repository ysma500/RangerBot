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
//Timer total 
#define TOTAL_TIME 175000
//Voltage du 5kHz
#define VOLTAGE_MICRO 1
// Include Files armus
#include <libarmus.h>
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
#define VALEUR_INTEG			2048

#define CTRL_GSSR				0
#define CTRL_GOFS				1

#define CONFIG_TOFS				0

//Couleurs des coins : Rose, vert, bleu, jaune, Centre : gris, contour : rouge
//Define des couleurs pour le capteurs de couleurs
#define RED_R 348
#define RED_G 152
#define RED_B 81
#define RED_C 414

#define GREY_R 239
#define GREY_G 280
#define GREY_B 215
#define GREY_C 590

#define YELLOW_R 608
#define YELLOW_G 567
#define YELLOW_B 220
#define YELLOW_C 896

#define PINK_R 535
#define PINK_G 348
#define PINK_B 258
#define PINK_C 798

#define GREEN_R 56
#define GREEN_G 107
#define GREEN_B 75
#define GREEN_C 196

#define BLUE_R 105
#define BLUE_G 142
#define BLUE_B 185
#define BLUE_C 284

#define WHITE_R 685
#define WHITE_G 755
#define WHITE_B 571
#define WHITE_C 896

//Code de chaque couleur
#define START_RED 1
#define START_GREY 2
#define START_YELLOW 3
#define START_PINK 4
#define START_GREEN 5
#define START_BLUE 6
#define START_WHITE 7
#define START_BLACK 8
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
void Avance(int iDistance);
void Rotation(float iAngle, int iDirection);
float PID_Setup(void);
void Tourne_gauche_avance();
void Tourne_gauche_avance(int valeurMod);
void Avance_BASE();

//Prototypes de fonctions de configs
void testDeCapteurs(int capteur[3]);
int capteurAffichage(int capteur[3]);
//Define
#define IR0 0	//Capteur plus bas
#define IR1 1	//Capteur niveau moyen
#define IR2 2	//Capteur plus haut
#define B1 1 //Analog input 1 à l'endroit B1
#define DISTANCE_MIN 100

int main()
{
	//Initialisation du capteur de couleur
	ERROR_CHECK(color_Init(adjd_dev));

	cap_SetValue(CAP_RED, 15);
	cap_SetValue(CAP_GREEN, 15);
	cap_SetValue(CAP_BLUE, 15);
	cap_SetValue(CAP_CLEAR, 15);

	integrationTime_SetValue(INTEGRATION_RED, VALEUR_INTEG);
	integrationTime_SetValue(INTEGRATION_GREEN, VALEUR_INTEG);
	integrationTime_SetValue(INTEGRATION_BLUE, VALEUR_INTEG);
	integrationTime_SetValue(INTEGRATION_CLEAR, VALEUR_INTEG);
	
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
	
	//Ecoute du 5kHz
	int condition_micro = 0;
	int micro_sound; //5kHz filtre
	int micro_background; //bruit de fond
	int micro_result;
	while (condition_micro == 0)
	{
		//ANALOG_Read(entre analogique) envoie un voltage 0 a 5 volt recu a un nombre entre 0 et 1023
		micro_sound = ANALOG_Read(6);
		micro_background = ANALOG_Read(7);
		micro_result = micro_sound - micro_background;
		THREAD_MSleep(1000);

		if (micro_result >= 20) //il faut choisir le VOLTAGE_MICRO, a tester experimentalement
		{
			LCD_Printf("Le signal de 5kHz a ete entendu \n");
			condition_micro = 1;
			m_iCouleurDep = get_current_color(); //trouver la couleur de depart
			LCD_Printf("La couleur ci-dessous est %d \n", m_iCouleurDep);
			Avance_BASE();
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
			Tourne_gauche_avance(START_RED);
		}
		else if(current_color == m_iCouleurDep)
		{
			LCD_Printf("COULEUR DE DEPART \n");
			Avance((-1,5 * MIN_DISTANCE));
			Rotation(180, 1);
		}
		else
		{
			//lecture des infrarouges
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
				case START_BLACK:
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
	if(clear < 130)
	{
		color = START_BLACK;
	}
	//Red
	else if (red >= (RED_R - LINE_HYST_PLUS) && red <= (RED_R + LINE_HYST_PLUS)
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

///Fonction tourne ou avance

void Tourne_gauche_avance()
{
	Rotation(45,LEFT_ROT);
	Avance(MIN_DISTANCE);
}

void Tourne_gauche_avance(int valeurMod)
{
	Rotation(45,LEFT_ROT);
	Avance(MIN_DISTANCE/3);
}

void Avance_BASE()
{
	Avance(MIN_DISTANCE);
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






/******************************************
 *
 *
 *
 * Fonctions Detecteurs Infrarouge
 *
 *
 *
 ***********************************/


/*

//Combinaison du multiplexeur A0,A1 et A2
int combinaison(int capteur)
{
	switch (capteur)
	{
			case 1 :
				DIGITALIO_Write(2, 0);
				DIGITALIO_Write(3, 0);
				DIGITALIO_Write(4, 0);
				break;

			case 2 :
				DIGITALIO_Write(2, 0);
				DIGITALIO_Write(3, 1);
				DIGITALIO_Write(4, 0);
				break;

			case 3 :
				DIGITALIO_Write(2, 1);
				DIGITALIO_Write(3, 1);
				DIGITALIO_Write(4, 0);
				break;
	}
	return 0;
}
int lireCapteur(int capteur_Infra[3])
{
	combinaison(IR0);
	capteur_Infra[IR0] = ANALOG_Read(B1);

	combinaison(IR1);
	capteur_Infra[IR1] = ANALOG_Read(B1);

	combinaison(IR2);
	capteur_Infra[IR2] = ANALOG_Read(B1);

}

int main()
{
	// variables locales
	int j = 0;
	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");


	//Nouveau contenu
	int capteur_Infra[3] = {0,0,0};			// 3 capteurs infra rouge utilisé pour la grande course


	while (j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(BMP_REAR))
		{
			LCD_Printf("Entrer dans les Tests de capteurs\n");
			j = 1;
		}
	}

	//Configuration
	testDeCapteurs(capteur_Infra);

	LCD_Printf("Fin du test du capteur\n");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}

//Debut de la fonction pour la modification des gains a suivre
void testDeCapteurs(int capteur[3])
{
	int i = 0, j = 0;

	int Affichage = 0;

	while(i==0 || j==0)
	{

		lireCapteur(capteur);

		/*
		LCD_Printf("Analog IR0 = %d\n", capteur[0]);
		LCD_Printf("Analog IR1 = %d\n", capteur[1]);
		LCD_Printf("Analog IR2 = %d\n", capteur[2]);
		*/

		//Si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT))	//Configuration 1 de la fonction de test des capteurs
		{
			capteurAffichage(capteur);
		}
		else if(DIGITALIO_Read(BMP_FRONT) && DIGITALIO_Read(BMP_RIGHT))	//Configuration 2 de la fonction de test des capteurs
		{

		}
		if(DIGITALIO_Read(BMP_REAR) && DIGITALIO_Read(BMP_FRONT))	//Sortie de la fonction de test des capteurs
		{
			i = 1;
			j = 1;
			LCD_Printf("Sortie des configs\n");
		}
	}
}
int capteurAffichage(int capteur[3])
{
	if( capteur[IR0] > DISTANCE_MIN || capteur[IR1] > DISTANCE_MIN || capteur[IR2] > DISTANCE_MIN )

		if( capteur[IR2] > capteur[IR1] && capteur[IR2] > capteur[IR0] )
		{
			LCD_ClearAndPrint("Detection d'un robot\n");
			LCD_Printf("Robot a : %d",capteur[IR2]);
		}

		else if( capteur[IR1] > capteur[IR0] && capteur[IR1] > capteur[IR2])
		{
			LCD_ClearAndPrint("Detection de la plateforme\n");
			LCD_Printf("La plateforme a : %d",capteur[IR1]);
		}

		else
		{
			LCD_ClearAndPrint("Objet detecter\n");
			LCD_Printf("L'objet a : %d",capteur[IR0]);
		}

	return 0;
}

*/



