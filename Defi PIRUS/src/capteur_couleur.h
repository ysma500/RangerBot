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
#define RED_R 553
#define RED_G 355
#define RED_B 275
#define RED_C 613

#define GREEN_R 111
#define GREEN_G 159
#define GREEN_B 118
#define GREEN_C 236

#define WHITE_R 818
#define WHITE_G 891
#define WHITE_B 718
#define WHITE_C 896

//Code de chaque couleur
#define RED 1
#define GREEN 2
#define OTHER 0

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


extern int adjd_dev;

#endif

