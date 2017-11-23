#ifndef CAPTEUR_COULEUR_H
#define CAPTEUR_COULEUR_H

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

//Fonction pour savoir la couleur au moment
int get_current_color();
void Init_Color(void);


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


extern int adjd_dev; //Works here but not in .h (weird)

#endif