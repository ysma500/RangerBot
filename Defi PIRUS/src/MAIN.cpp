///Main.cpp
// Include Files
#include <libarmus.h>
#include "infrarouge.h"
#include "sonar.h"
#include "boutons.h"
#include "capteur_couleur.h"

int autre_test();
int sonar_test();
int color_test();
int infra_test();
int boutons_test();

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
		
		//Appuyer le BUMP_REAR pour avoir le bon menu
		while (SYSTEM_ReadTimerMSeconds() < 5000)
		{
			if(DIGITALIO_Read(BMP_REAR))
			{
				menu_option++;
				LCD_Printf("Item selectionner: %i \r", menu_option);
				THREAD_MSleep(50);
			}
		}
		
		switch(menu_option)
		{
			case 1 :
				LCD_Printf("1. Demonstration sonar\n");
				sonar_test();
				
				THREAD_MSleep(2000);
				LCD_Printf("Retour au menu dans 2 secondes \n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			case 2 : 
				LCD_Printf("2. Demonstration detecteur de couleurs\n");
				color_test();
				
				THREAD_MSleep(2000);
				LCD_Printf("Retour au menu dans 2 secondes \n");
				THREAD_MSleep(2000);
				menu_option = 0; //Retour au menu de selection apres demo
				break;
			case 3 : 
				LCD_Printf("3. Demonstration infrarouge\n");
				infra_test();
				
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

//Fonctions de demo
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

int sonar_test()
{
	LCD_ClearAndPrint("Depart du test des sonars\n");
	test_de_sonar(); //Il faut appuyer sur le BUMP_REAR pour arreter.
	
	LCD_Printf("Le robot a termine le test\n");
	//Attendre 5 secondes et effacer ce qu'il y a a l'ecran
	THREAD_MSleep(5000);
	LCD_ClearAndPrint("");
	return 0;
}

int color_test()
{
	int current_color;
	int condition = 0;
	LCD_ClearAndPrint("Mesure dans 5 secondes! \n");
	current_color = get_current_color();
	switch(current_color)
		{
			case START_RED:
				LCD_Printf("RED \n");
				break;
			case START_GREY:
				LCD_Printf("GREY \n");
				break;
			case START_YELLOW:
				LCD_Printf("YELLOW \n");
				break;
			case START_PINK:
				LCD_Printf("PINK \n");
				break;
			case START_GREEN:
				LCD_Printf("GREEN \n");
				break;
			case START_BLUE:
				LCD_Printf("BLUE \n");			
				break;
			case START_WHITE:
				LCD_Printf("WHITE \n");
				break;
			default:
				LCD_Printf("I don't know where the fuck I am\n");
				break;
		}
	LCD_Printf("Fin du test du capteur de couleurs\n");
	
	// Le code attent 5 secondes
	THREAD_MSleep(5000);
	LCD_ClearAndPrint("");
	return 0;
}

int infra_test()
{
	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du test des infrarouge\n");
	
	//Definir tes entree (les 3 capteurs utilisee)
	int capteur_Infra[3] = {0,0,0}; 
	
	//Test
	testDeCapteurs(capteur_Infra);
	
	LCD_Printf("Fin du test du capteur infrarouge\n");
	
	// Le code attent 5 secondes
	THREAD_MSleep(5000);
	LCD_ClearAndPrint("");
	return 0;
}

int boutons_test()
{
	LCD_ClearAndPrint("Depart du test des boutons\n");
	//Nouveau contenu
	int code[NB_CODE_MAX] = {0,0,0,0,0};
	test_des_boutons(code);
	
	LCD_Printf("Fin du test des boutons\n");

	// Le code attent 5 secondes
	THREAD_MSleep(5000);
	LCD_ClearAndPrint("");
	return 0;
}
