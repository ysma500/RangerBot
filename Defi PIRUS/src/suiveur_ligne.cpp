// Include Files
#include <libarmus.h>
#include"suiveur_ligne.h"

int detection_ligne()
{
	int boolean_line;
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
			MOTOR_SetSpeed(MOTOR_DROIT,-70);
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



