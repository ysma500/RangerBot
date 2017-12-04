// Include Files
#include <libarmus.h>
#include "infrarouge.h"

//Combinaison du multiplexeur A0,A1 et A2
int combinaison(int capteur)
{
	switch (capteur)
	{
			case 0 :
				DIGITALIO_Write(2, 0);
				DIGITALIO_Write(3, 0);
				DIGITALIO_Write(4, 0);
				break;

			case 1 :
				DIGITALIO_Write(2, 1);
				DIGITALIO_Write(3, 0);
				DIGITALIO_Write(4, 0);
				break;

			case 2 :
				DIGITALIO_Write(2, 0);
				DIGITALIO_Write(3, 1);
				DIGITALIO_Write(4, 0);
				break;
	}
	return 0;
}

//Place la valeur Analog_read dans le tableau capteur infra
int lireCapteur(int* IRAVANT, int* IRRIGHT, int* IRLEFT)
{
	combinaison(IR0);
	*IRAVANT = ANALOG_Read(B1);

	combinaison(IR1);
	*IRRIGHT = ANALOG_Read(B1);

	combinaison(IR2);
	*IRLEFT = ANALOG_Read(B1);
	return 0;
}
/*
//Debut de la fonction pour la modification des gains a suivre 
void afficher_IR(int capteur[3])
{
	int i = 0;
	
	while(i==0)
	{
		//Aller chercher les valeurs Analog read de chaque capteur infrarouge
		//lireCapteur(&capteur);

		//Afficher les valeurs analog read 
		LCD_ClearAndPrint("Analog IR0 = %d\n", capteur[0]);
		LCD_Printf("Analog IR1 = %d\n", capteur[1]);
		LCD_Printf("Analog IR2 = %d\n\n", capteur[2]);
		
		//capteurAffichage(capteur); //Afficher la distance et quel objet est devant nous calibre pour grande course
		
		THREAD_MSleep(250);
		if(DIGITALIO_Read(BMP_REAR))
		{
			LCD_Printf("Sortie de lecture des infra");
			i = 1;
		}
	}
}
*/
/*
int capteurAffichage(int capteur[3])
{
	//if( capteur[IR0] > DISTANCE_MIN || capteur[IR1] > DISTANCE_MIN || capteur[IR2] > DISTANCE_MIN )

		if((capteur[IR0] >= capteur[IR1]) && (capteur[IR0] >= capteur[IR2]))
		{
			LCD_Printf("Objet detecter\n");
			LCD_Printf("L'objet a : %d\n",capteur[IR0]);
		}
		else if((capteur[IR1] >= capteur[IR2]) && (capteur[IR0] >= capteur[IR2]))
		{
			LCD_Printf("Detection de la plateforme\n");
			LCD_Printf("La plateforme a : %d\n",capteur[IR1]);
		}
		else
		{
			LCD_Printf("Detection d'un robot\n");
			LCD_Printf("Robot a : %d\n",capteur[IR2]);
		}

	return 0;
}
*/



