#include "CapteurDeCouleur.h"
#include <libarmus.h>

//Valeur de couleur de depart
int m_iCouleurDep;

//Configuration du capteur de couleur
void Init_Colors(void)
{
	//Initialisation du capteur de couleur
	ERROR_CHECK(color_Init(adjd_dev));
	
	m_iCouleurDep = 0;

	cap_SetValue(CAP_RED, 15);
	cap_SetValue(CAP_GREEN, 15);
	cap_SetValue(CAP_BLUE, 15);
	cap_SetValue(CAP_CLEAR, 15);

	integrationTime_SetValue(INTEGRATION_RED, 255);
	integrationTime_SetValue(INTEGRATION_GREEN, 255);
	integrationTime_SetValue(INTEGRATION_BLUE, 255);
	integrationTime_SetValue(INTEGRATION_CLEAR, 255);
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
