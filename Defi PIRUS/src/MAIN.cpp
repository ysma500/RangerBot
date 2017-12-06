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
#define MODE_STANDBY 0
#define MODE_PASSIF 1
#define MODE_ACTIF 2
#define CHANGER_CODE 3
#define TEST_ALARME 4

//DELS
#define DEL_OFF 1
#define DEL_ON 0
#define DEL_ROUGE 11
#define DEL_VERT 10
#define DEL_BLEU 12


int passif();
int actif();
int code();
int selection_mode();
int test_alarme();

int mot_passe[LONGUEUR_CODE] = {1, 1, 1, 1};

int main()
{
	// Variables locales
	int j = 0; 			  //Condition pour rentrer dans le programme
	int menu_option = 0; //Option de menu pour la demonstration
	int distance_percue; //Distance lue par les sonars
	int distance_max = 10; 	 //Distance max avant de tourner lorsqu'on arrive a un mur (en cm)
	int mode = MODE_STANDBY; //current mode du robot 
	int speaker_tester = 0;
	int standby_standby = 0;
	
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
	/*
	LCD_ClearAndPrint("Test des speakers! \n");
	play_setup();
	LCD_ClearAndPrint("Fin du test des speakers! \n");
	THREAD_MSleep(3000);
	LCD_ClearAndPrint("");
	*/
	//DELS
	//ROUGE : 11
	//BLEU : 12
	//VERT : 10
	
	while (1)
	{
	//Mise a zero des variables de sonars
	last_sonar_d = 0;
	last_sonar_g = 0;
	flag_brigand = 0;
	//On met tous les dels a zero
	DIGITALIO_Write(DEL_BLEU, DEL_OFF);
	DIGITALIO_Write(DEL_ROUGE, DEL_OFF);
	DIGITALIO_Write(DEL_VERT, DEL_OFF);
	
	//Entree dans les menus
		THREAD_MSleep(1230);
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
			case MODE_STANDBY :
				LCD_ClearAndPrint("Presentement dans le mode standby! \n Pour retourner dans le menu de selection,\n appuyer sur le bouton bleu.");
				standby_standby = 0;
				while (standby_standby == 0)
				{
					if (DIGITALIO_Read(BMP_BLEU))
					{
						standby_standby = 1;
					}
				}
			case TEST_ALARME :
				test_alarme();
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


//couleurs : RED, GREEN, OTHER
int code()
{
	LCD_ClearAndPrint("Presentement dans le mode changement de code\n");
	nouveau_code(mot_passe);
	show_code(mot_passe);
	LCD_Printf("\nNouveau code recu, retour au menu dans 3 secondes");
	THREAD_MSleep(3000);
	LCD_ClearAndPrint("");
	return 0;
}

int test_alarme()
{
	if (entrer_code(mot_passe) == 0)
	{
		LCD_ClearAndPrint("Mauvais code!\n");
		LCD_Printf("Appuyer sur le bouton orange gauche pour arreter l'alarme");
		play_siren();
	}
	else
	{
		LCD_ClearAndPrint("Bon code! Bravo!");
	}
	return 0;
}

int passif()
{
	LCD_ClearAndPrint("Presentement dans le mode passif\n");
	int current_color;
	int condition_mode = 0;
	int brigand_present = 0;

	while (condition_mode == 0)
	{
		brigand_present = brigand_passif();
		if (brigand_present)
		{
			DIGITALIO_Write(DEL_BLEU, DEL_ON);
			if (entrer_code(mot_passe) == 0)
				{
					LCD_ClearAndPrint("Mauvais code!\n");
					LCD_Printf("Appuyer sur le bouton orange a gauche pour arreter l'alarme");
					DIGITALIO_Write(DEL_ROUGE, DEL_ON);
					DIGITALIO_Write(DEL_BLEU, DEL_OFF);
					play_siren();
					condition_mode = 1;
				}
				else
				{
					DIGITALIO_Write(DEL_VERT, DEL_ON);
					LCD_ClearAndPrint("Bon code! Bravo!");
					condition_mode = 1;
				}
		}
		THREAD_MSleep(300);
		if (DIGITALIO_Read(ORANGE_RIGHT))
		{
			condition_mode = 1;
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
	int brigand_present = 0;
	while (condition_mode == 0)
	{
		current_color = get_current_color();
		switch (current_color)
		{
			case RED :
				LCD_ClearAndPrint("ROUGE Detecte\n");
				//Recule,tourne à gauche 90 deg. et avance
				Avance(MIN_DISTANCE*2, RECULE);
				Rotation(105,LEFT_ROT);
				Avance(520, AVANCE);
				Rotation(90,RIGHT_ROT);
				Avance(200, AVANCE);
				break;
			default :
				//Detection d'un intrus
				brigand_present = suivre_brigand();
				if(!brigand_present)
				{
					Mouv_infra();
				}
				if (brigand_present)
				{
					DIGITALIO_Write(DEL_BLEU, DEL_ON);
					if (entrer_code(mot_passe) == 0)
						{
							LCD_ClearAndPrint("Mauvais code!\n");
							LCD_Printf("Appuyer sur le bouton orange a gauche pour arreter l'alarme");
							DIGITALIO_Write(DEL_ROUGE, DEL_ON);
							DIGITALIO_Write(DEL_BLEU, DEL_OFF);
							play_siren();
							condition_mode = 1;
						}
						else
						{
							DIGITALIO_Write(DEL_VERT, DEL_ON);
							LCD_ClearAndPrint("Bon code! Bravo!");
							condition_mode = 1;
						}
				}
				break;
		}
		THREAD_MSleep(100);
		if (DIGITALIO_Read(ORANGE_RIGHT))
		{
			Brake();
			condition_mode = 1;
		}
	}
	THREAD_MSleep(3000);
	return 0;
}

//Fonctions de chaque mode
int selection_mode()
{
	int option = -1;
	int j = 0, i = 0;
	while (j==0)
	{
		if (option == -1)
		{
			LCD_ClearAndPrint("Pour entrer dans le mode passif, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			while(i == 0)
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
			LCD_ClearAndPrint("Pour tester le systeme d'alarme, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			i = 0;
			while(i == 0)
			{
				if (DIGITALIO_Read(ORANGE_RIGHT))
				{
					LCD_ClearAndPrint("");
					i = 1;
					j = 1;
					option = 4;
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
		
		LCD_ClearAndPrint("");
		
		if (option == -1)
		{
			LCD_ClearAndPrint("Pour entrer dans le mode standby, appuyer sur le bouton orange de droite\n");
			LCD_Printf("Pour changer d'option appuyer sur le bouton orange de gauche\n");
			i = 0;
			while(i == 0)
			{
				if (DIGITALIO_Read(ORANGE_RIGHT))
				{
					LCD_ClearAndPrint("");
					i = 1;
					j = 1;
					option = 0;
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

