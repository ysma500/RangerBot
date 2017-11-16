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
/******************************************
 *
 *
 *
 *
 *
 *
 *
 ***********************************/
// Include Files
#include <libarmus.h>

//Define
#define IR0 0	//Capteur plus bas
#define IR1 1	//Capteur niveau moyen
#define IR2 2	//Capteur plus haut
#define B1 1 //Analog input 1 � l'endroit B1
#define DISTANCE_MIN 100

#define NB_CODE_MAX 5
#define RIEN 0
#define BTN_1 1
#define BTN_2 2
#define BTN_3 3
#define BTN_4 4

// Prototypes de fonctions de configs
void testDeCapteurs(int code[NB_CODE_MAX]);
int creerUnCode(int code[NB_CODE_MAX]);
int valeurBumper(void);

int main()
{
	// variables locales
	int j = 0;
	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");

	
	//Nouveau contenu
	int code[NB_CODE_MAX];


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
	testDeCapteurs(code);
	
	LCD_Printf("Fin du test du capteur\n");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}

//Debut de la fonction pour la modification des gains a suivre 
void testDeCapteurs(int code[NB_CODE_MAX])
{
	int i = 0, j = 0;

	int Affichage = 0;

	while(i==0 || j==0)
	{
		
		//Si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT))	//Configuration 1 de la fonction de test des capteurs
		{
			LCD_ClearAndPrint("Attente de 2 secondes avant le test \n");
			THREAD_MSleep(2000);
			creerUnCode(code);
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
		THREAD_MSleep(1000);
	}
}
int creerUnCode(int code[NB_CODE_MAX])
{
	//Premier emplacement du tableau donne la longueur du code

	int valeur = 0;
	int nb_Code = 0;
	int i;
	LCD_ClearAndPrint("Uni test pour les boutons : \n");
	while(1)
	{
		valeur = valeurBumper();

		if(valeur > 0)
		{

			nb_Code ++;
			code[0] = nb_Code;
			code[nb_Code] = valeur;
			valeur = 0;
			LCD_Printf("Chiffre : %d \n", code[nb_Code]);
		}

		if(nb_Code >= NB_CODE_MAX - 1)
		{
			LCD_ClearAndPrint("Les codes entrees sont : \n");
			LCD_Printf("Vous avez entree %d codes !\n",code[0]);

			for(i = 1; i < NB_CODE_MAX ; i++)
			{
				LCD_Printf("Le %d code est : %d \n", i, code[i]);

			}

			LCD_Printf("\n\nAffichage avant la fin du test 2 secondes");
			THREAD_MSleep(2000);
			LCD_Printf("\nFin");
			break;
		}

	}

}

int valeurBumper(void)
{

	if(DIGITALIO_Read(BMP_FRONT))
	{
		return BTN_1;
	}
	else if(DIGITALIO_Read(BMP_RIGHT))
	{
		return BTN_2;
	}
	else if(DIGITALIO_Read(BMP_REAR))
	{
		return BTN_3;
	}
	else if(DIGITALIO_Read(BMP_RIGHT))
	{
		return BTN_4;
	}

	return RIEN;
}




