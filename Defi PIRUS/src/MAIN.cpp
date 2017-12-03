///Main.cpp
// Include Files
#include <libarmus.h>
#include "infrarouge.h"
#include "sonar.h"
#include "boutons.h"
#include "capteur_couleur.h"
#include "mouvement.h"


//Define pour les modes
#define MODE_DEPART 0
#define MODE_PASSIF 1
#define MODE_ACTIF 2
#define CHANGER_CODE 3

//Define pour les boutons
#define ORANGE_LEFT 8
#define ORANGE_RIGHT 9


int infra_test();
int boutons_test();
int passif();
int actif();
int code();
int selection_mode();

int main()
{
	// variables locales
	int j = 0; 			  //Condition pour rentrer dans le programme
	int menu_option = 0; //Option de menu pour la demonstration
	int distance_percue; //Distance lue par les sonars
	int distance_max = 10; 	 //Distance max avant de tourner lorsqu'on arrive a un mur (en cm)
	int mode = MODE_DEPART; //current mode du robot 
	//Initialisation du capteur de couleurs
	Init_Color();
	
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
	/*
	Menu pour selectionner entre : 
	1. Mode passif
	2. Mode actif
	3. Changer de code / parametres
	On selectionne avec le bouton orange a gauche et on confirme avec le bouton orange a droite
	On doit pooler apres chaque passe des capteurs pour voir si on veut changer de mode. 
	*/
	//Pour sonars : sonar_g() et sonar_d() retournent la distance percue par chaque capteur
	
	while (1)
	{
		mode = selection_mode();
		switch (mode)
		{
			case MODE_PASSIF : 
				
				break;
			case MODE_ACTIF : 
				
				break;
			case CHANGER_CODE :
				
				break;
			case 4 :
				
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

int infra_test()
{
	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du test des infrarouge\n");
	
	//Definir tes entree (les 3 capteurs utilisee)
	int lectures_INFRA[3] = {0,0,0}; 
	
	//Test
	testDeCapteurs(lectures_INFRA);
	
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
		
		if (option == -1)
		{
			LCD_ClearAndPrint("Pour entrer dans le mode actif, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			i = 0;
			while(i == 0)
			{
				if (DIGITALIO_Read(ORANGE_RIGHT))
				{
					i = 1;
					j = 1;
					option = 2;
				}
				else if(DIGITALIO_Read(ORANGE_LEFT))
				{
					i = 1;
				}	
			THREAD_MSleep(100);
			}		
		}
		
		if (option == -1)
		{
			LCD_ClearAndPrint("Pour changer le mot de passe du robot, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			i = 0;
			while(i == 0)
			{
				if (DIGITALIO_Read(ORANGE_RIGHT))
				{
					i = 1;
					j = 1;
					option = 3;
				}
				else if(DIGITALIO_Read(ORANGE_LEFT))
				{
					i = 1;
				}	
			THREAD_MSleep(100);
			}		
		}
		
		if (option == -1)
		{
			LCD_ClearAndPrint("Pour ANNULER tout changement, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			i = 0;
			while(i == 0)
			{
				if (DIGITALIO_Read(ORANGE_RIGHT))
				{
					i = 1;
					j = 1;
					option = 4;
				}
				else if(DIGITALIO_Read(ORANGE_LEFT))
				{
					i = 1;
				}	
			THREAD_MSleep(100);
			}		
		}
	}
	
	return option;
}
