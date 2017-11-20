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
/*
float GAIN_I = 0.223;
float GAIN_P = 1.68;
int m_iTicTotalG = 0;
int m_iTicTotalD = 0;
*/
int autre_test();



int main()
{
	
	// variables locales
	int j = 0; //Condition pour rentrer dans le programme
	int menu_option = 0; //Option de menu pour la demonstration

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);
	LCD_ClearAndPrint("Appuyer sur le bumper arriere pour commencer le programme\n");
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
	
	
	while(menu_option != 4)
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
				LCD_Printf("Retour au menu dans 2 secondes \n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			case 2 : 
				LCD_Printf("2. Demonstration detecteur de couleurs\n");
				
				THREAD_MSleep(2000);
				LCD_Printf("Retour au menu dans 2 secondes \n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			case 3 : 
				LCD_Printf("3. Demonstration infrarouge\n");
				
				THREAD_MSleep(2000);
				LCD_Printf("Retour au menu dans 2 secondes \n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			default :
				LCD_Printf("4. Sortie\n");
				
				THREAD_MSleep(5000);
				if(autre_test() == 1)
				{
					menu_option = 0;
					LCD_Printf("Retour au menu dans 2 secondes \n");
					THREAD_MSleep(2000);
				}
				
				break;
		}
	}
	

	// Le code attent 10 secondes
	THREAD_MSleep(10000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}


int autre_test()
{
	int condition = 0;
	LCD_ClearAndPrint("Voulez vous faire une autre demo?\n Si oui, tenir le bumbper arriere.\n");
	THREAD_MSleep(500);
	while (condition == 0)
	{
		if(DIGITALIO_Read(BMP_REAR))
		{
			condition = 1;
		}
		THREAD_MSleep(200);
	}
	return condition;
	
}


