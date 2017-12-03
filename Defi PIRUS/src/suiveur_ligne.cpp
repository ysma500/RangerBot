// Include Files
#include <libarmus.h>
#include"ligne.h"

// Define pour la fonction
#define LINE_DROIT 568
#define LINE_CENTRE 291
#define LINE_GAUCHE 150
#define LINE_DROIT_CENTRE 849
#define LINE_DROIT_GAUCHE 712
#define LINE_CENTRE_GAUCHE 436
#define LINE_NONE 5
#define LINE_ALL 994
#define LINE_HYST 10

// Prototypes de fonctions de configs
//void testDeCapteurs(void);
void det_ligne(void);

int main()
{
	// variables locales
	int j = 0;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");
	
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
	//testDeCapteurs();
	det_ligne();
	
	LCD_Printf("Fin du test du capteur\n");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}


//Debut de la fonction pour la modification des gains a suivre 
void det_ligne(void)
{
	int i = 1;
	int valeur_lignes = 0;
	while(i ==1)
	{

		valeur_lignes = ANALOG_Read(2);

		//Ligne a droite
		if((valeur_lignes <= (LINE_CENTRE_GAUCHE + LINE_HYST)) && (valeur_lignes >= (LINE_CENTRE_GAUCHE - LINE_HYST)))
		{
			MOTOR_SetSpeed(MOTOR_DROIT,70);
			MOTOR_SetSpeed(MOTOR_GAUCHE,60);
		}
		¸<
		¸
		¸.

		else if((valeur_lignes < (LINE_DROIT_CENTRE + LINE_HYST)) && (valeur_lignes > (LINE_DROIT_CENTRE - LINE_HYST)))
		{
			MOTOR_SetSpeed(MOTOR_DROIT,70);
			MOTOR_SetSpeed(MOTOR_GAUCHE,60);
		}
		else if((valeur_lignes < (LINE_DROIT_GAUCHE + LINE_HYST)) && (valeur_lignes > (LINE_DROIT_GAUCHE - LINE_HYST)))

		{
			MOTOR_SetSpeed(MOTOR_GAUCHE,70);
			MOTOR_SetSpeed(MOTOR_DROIT,70);
		}
		else if((valeur_lignes < (LINE_DROIT + LINE_HYST)) && (valeur_lignes > (LINE_DROIT - LINE_HYST)))
		{
			MOTOR_SetSpeed(MOTOR_GAUCHE,70);
			MOTOR_SetSpeed(MOTOR_DROIT,65);
		}
		else if((valeur_lignes < (LINE_GAUCHE + LINE_HYST)) && (valeur_lignes > (LINE_GAUCHE - LINE_HYST)))
		{
			MOTOR_SetSpeed(MOTOR_DROIT,70);
			MOTOR_SetSpeed(MOTOR_GAUCHE,65);
		}
		else if((valeur_lignes < (LINE_CENTRE + LINE_HYST)) && (valeur_lignes > (LINE_CENTRE - LINE_HYST)))
		{
			MOTOR_SetSpeed(MOTOR_GAUCHE,70);
			MOTOR_SetSpeed(MOTOR_DROIT,70);
		}
		else if((valeur_lignes < (LINE_NONE + LINE_HYST)) && (valeur_lignes > (LINE_NONE - LINE_HYST)))
		{
			MOTOR_SetSpeed(MOTOR_GAUCHE,70);
			MOTOR_Se0002
			02tSpeed(MOTOR_DROIT,-70);
		}
		else if((valeur_lignes < (LINE_ALL + LINE_HYST)) && (valeur_lignes > (LINE_ALL - LINE_HYST)))
		{
			i = 0;
		}
	THREAD_MSleep(250);
	}
}


/*/Debut de la fonction pour la modification des gains a suivre
void testDeCapteurs(void)
{
	int i = 0, j = 0;
	int valeurLigne = 0;



	while(i==0 || j==0)
	{

		valeurLigne = ANALOG_Read(2);

		//Si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT))	//Configuration 1 de la fonction de test des capteurs
		{
			LCD_ClearAndPrint("Valeur analogique des capteurs de ligne : %d\n", valeurLigne);
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
	}
}
*/



