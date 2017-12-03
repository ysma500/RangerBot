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



