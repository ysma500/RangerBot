///Main.cpp
// Include Files
#include <libarmus.h>
#include "infrarouge.h"
#include "sonar.h"
#include "boutons.h"
#include "capteur_couleur.h"
#include "mouvement.h"
#include "speaker.h"


//Define pour les modes
#define MODE_DEPART 0
#define MODE_PASSIF 1
#define MODE_ACTIF 2
#define CHANGER_CODE 3


int infra_test();
int boutons_test();
int passif();
int actif();
int code();
int selection_mode();

int main()
{
	// Variables locales
	int j = 0; 			  //Condition pour rentrer dans le programme
	int menu_option = 0; //Option de menu pour la demonstration
	int distance_percue; //Distance lue par les sonars
	int distance_max = 10; 	 //Distance max avant de tourner lorsqu'on arrive a un mur (en cm)
	int mode = MODE_DEPART; //current mode du robot 
	int speaker_tester = 0;
	//Initialisation du capteur de couleurs
	Init_Color();
	
	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);
	LCD_ClearAndPrint("Appuyer sur le bouton bleu pour commencer le programme\n");
	while (j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(BMP_BLEU))
		{
			LCD_Printf("Depart du programme");
			j = 1;
		}
	}
	
	/*
	Menu pour selectionner entre : 
	1. Mode passif
	2. Mode actif
	3. Changer de code / parametres
	On selectionne avec le bouton orange a gauche et on confirme avec le bouton orange a droite
	On doit pooler apres chaque passe des capteurs pour voir si on veut changer de mode. 
	*/
	//Pour sonars : sonar_g() et sonar_d() retournent la distance percue par chaque capteur
	
	LCD_ClearAndPrint("Test des speakers! \n");
	play_setup();
	LCD_ClearAndPrint("Fin du test des speakers! \n");
	THREAD_MSleep(3000);
	LCD_ClearAndPrint("");
	
	while (1)
	{
		mode = selection_mode();
		switch (mode)
		{
			case MODE_PASSIF : 
				passif();
				break;
			case MODE_ACTIF : 
				actif();
				break;
			case CHANGER_CODE :
				code();
				break;
			default :
				LCD_ClearAndPrint("Aucun cas selectionne!\n");
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


//Fonctionnalites individuelles
int infra_test()
{
	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du test des infrarouge\n");
	
	//Definir tes entree (les 3 capteurs utilisee)
	int lectures_INFRA[3] = {0,0,0}; 
	
	//Test
	afficher_IR(lectures_INFRA);
	
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

//Fonctions de chaque mode
int selection_mode()
{
	int option = -1;
	int j = 0, i = 0;
	while (j==0)
	{
		LCD_ClearAndPrint("Pour entrer dans le mode passif, appuyer sur le bouton orange de droite\n");
		LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
		while(i==0)
		{
			if (DIGITALIO_Read(ORANGE_RIGHT))
			{
				i = 1;
				j = 1;
				option = 1;
			}
			else if(DIGITALIO_Read(ORANGE_LEFT))
			{
				i = 1;
			}	
			THREAD_MSleep(100);
		}
		
		LCD_ClearAndPrint("");
		
		if (option == -1)
		{
			LCD_ClearAndPrint("Pour entrer dans le mode actif, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			i = 0;
			while(i == 0)
			{
				if (DIGITALIO_Read(ORANGE_RIGHT))
				{
					LCD_ClearAndPrint("");
					i = 1;
					j = 1;
					option = 2;
				}
				else if(DIGITALIO_Read(ORANGE_LEFT))
				{
					LCD_ClearAndPrint("");
					i = 1;
				}	
			THREAD_MSleep(100);
			}		
		}
		
		LCD_ClearAndPrint("");
		
		if (option == -1)
		{
			LCD_ClearAndPrint("Pour changer le mot de passe du robot, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			i = 0;
			while(i == 0)
			{
				if (DIGITALIO_Read(ORANGE_RIGHT))
				{
					LCD_ClearAndPrint("");
					i = 1;
					j = 1;
					option = 3;
				}
				else if(DIGITALIO_Read(ORANGE_LEFT))
				{
					LCD_ClearAndPrint("");
					i = 1;
				}	
			THREAD_MSleep(100);
			}		
		}
	}//end of while (j)
	
	return option;
}

//couleurs : START_RED, START_GREEN, START_OTHER
int code()
{
	LCD_ClearAndPrint("Presentement dans le mode changement de code\n");
	
	THREAD_MSleep(3000);
	return 0;
}

int passif()
{
	LCD_ClearAndPrint("Presentement dans le mode passif\n");
	int current_color;
	int condition_mode = 0;
	while (condition_mode == 0)
	{
		current_color = get_current_color();
		switch (current_color)
		{
			case GREEN : 
				
				break;
			case RED : 
				
				break;
			case OTHER :
				
				break;
		}
	}
	THREAD_MSleep(3000);
	return 0;
}

int actif()
{
	LCD_ClearAndPrint("Presentement dans le mode actif\n");
	int current_color;
	int condition_mode = 0;
	while (condition_mode == 0)
	{
		current_color = get_current_color();
		switch (current_color)
		{
			case RED :
				LCD_ClearAndPrint("ROUGE Detecte\n");
				break;
			default : 
				break;
				
		}
	}
	THREAD_MSleep(3000);
	return 0;
}
