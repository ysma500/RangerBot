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

//***************************************
//Block d'information
//Necessaires pour le capteur de couleurs
//***************************************
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
#define RED_R 544
#define RED_G 266
#define RED_B 184
#define RED_C 887

#define GREY_R 560
#define GREY_G 632
#define GREY_B 510
#define GREY_C 1024

#define YELLOW_R 626
#define YELLOW_G 558
#define YELLOW_B 365
#define YELLOW_C 910

#define PINK_R 780
#define PINK_G 475
#define PINK_B 390
#define PINK_C 1015

#define GREEN_R 265
#define GREEN_G 355
#define GREEN_B 223
#define GREEN_C 617

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

#define LINE_HYST_PLUS 100 //incertitude sur jaune et rouge
#define LINE_HYST 50 //Incertitude sur la detection de couleurs

//Fonction pour savoir la couleur au moment
int get_current_color();

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

//***************************************
//FIN Block d'information FIN
//Necessaires pour le capteur de couleurs
//***************************************

// Prototypes de fonctions de configs
int testDeCapteurs(void);
int afficher_live(void);

int main()
{
	//initialisation du capteur
	ERROR_CHECK(color_Init(adjd_dev));

	cap_SetValue(CAP_RED, 15);
	cap_SetValue(CAP_GREEN, 15);
	cap_SetValue(CAP_BLUE, 15);
	cap_SetValue(CAP_CLEAR, 15);

	integrationTime_SetValue(INTEGRATION_RED, VALEUR_INTEG);
	integrationTime_SetValue(INTEGRATION_GREEN, VALEUR_INTEG);
	integrationTime_SetValue(INTEGRATION_BLUE, VALEUR_INTEG);
	integrationTime_SetValue(INTEGRATION_CLEAR, VALEUR_INTEG);

	// variables locales
	int j = 0;
	int current_color;
	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);
	
	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");
	LCD_Printf("Appuyer sur le bouton bleu pour afficher les valeurs lues\n");
		while (j == 0)
		{
			THREAD_MSleep(100);
			if(DIGITALIO_Read(BMP_FRONT))
			{
				LCD_Printf("Entrer dans les tests des capteurs\n");
				j = 1;
			}
			THREAD_MSleep(100);
		}

		if (j == 1)
		{
			testDeCapteurs();
			LCD_Printf("Fin du test du capteur\n");
		}
		if (j == 2)
		{
				current_color = afficher_live();
		}
	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");
	return 0;
}

int afficher_live()
{
	int j = 0;
	int color;

	while(j==0)
	{
		color = get_current_color();
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
	}
		THREAD_MSleep(1000);
	return 0;
}

//Debut de la fonction pour la modification des gains a suivre
int testDeCapteurs(void)
{
	int i = 0, j = 0;
	int red, blue, green, clear;

	while(i==0 || j==0)
	{
		color_Read(red, blue, green, clear);
		if(DIGITALIO_Read(BMP_REAR))
		{
			i = 1;
			j = 1;
			LCD_Printf("Sortie des configs\n");
		}
		LCD_ClearAndPrint("R=%d, G=%d, B=%d, C=%d \n", red, green, blue, clear);
		THREAD_MSleep(1000);
	}
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
