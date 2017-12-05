// Include Files
#include <libarmus.h>
#include"suiveur_ligne.h"
#include"mouvement.h"


//Debut de la fonction pour la modification des gains a suivre
int det_ligne()
{
	int valeur_lignes = 0;
	valeur_lignes = ANALOG_Read(2);

	return valeur_lignes;
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


int ligne_centre() // retourne 1 si centre, retourne -1 si a perdu la ligne
{
	int flag_centre = 0;
	while (flag_centre == 0)
	{
			int position = 0;
			position = det_ligne();
			if ((position < LINE_DROIT - LINE_HYST) && (position > LINE_DROIT + LINE_HYST))
			{ //Trop a droite
				Rotation(25, LEFT_ROT);
				Avance(MIN_DISTANCE, AVANCE);
				Rotation(25, RIGHT_ROT);
				Avance(MIN_DISTANCE, RECULE);
			}
			else if ((position < LINE_GAUCHE - LINE_HYST) && (position > LINE_GAUCHE + LINE_HYST))
			{ //Trop a gauche
				Rotation(20, RIGHT_ROT);
				Avance(MIN_DISTANCE, AVANCE);
				Rotation(20, LEFT_ROT);
				Avance(MIN_DISTANCE, RECULE);
			}
			else if ((position < LINE_DROIT_CENTRE - LINE_HYST) && (position > LINE_DROIT_CENTRE + LINE_HYST))
			{ //un peu trop a droite
				Rotation(15, LEFT_ROT);
				Avance(MIN_DISTANCE, AVANCE);
				Rotation(15, RIGHT_ROT);
				Avance(MIN_DISTANCE, RECULE);
			}
			else if ((position < LINE_GAUCHE_CENTRE - LINE_HYST) && (position > LINE_GAUCHE_CENTRE + LINE_HYST))
			{ //un peu trop a gauche
				Rotation(10, RIGHT_ROT);
				Avance(MIN_DISTANCE, AVANCE);
				Rotation(10, LEFT_ROT);
				Avance(MIN_DISTANCE, RECULE);
			}
			else if ((position < LINE_CENTRE - LINE_HYST) && (position > LINE_CENTRE + LINE_HYST))
			{
				flag_centre = 1;
				break;
			}
			else if (position < 50)
			{
				flag_centre = -1;
				LCD_Printf ("Ligne perdue :( \n");
				break;
			}
	}
	return flag_centre;
}
