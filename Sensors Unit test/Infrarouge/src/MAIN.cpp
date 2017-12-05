

// Include Files
#include <libarmus.h>

// Prototypes de fonctions de configs
void testDeCapteurs(int capteur[3]);
int capteurAffichage(int capteur[3]);

//Define
#define IR0 0	//Capteur plus bas
#define IR1 1	//Capteur niveau moyen
#define IR2 2	//Capteur plus haut
#define B1 1 //Analog input 1 à l'endroit B1
#define DISTANCE_MIN 100

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
int lireCapteur(int capteur_Infra[3])
{
	combinaison(IR0);
	capteur_Infra[IR0] = ANALOG_Read(B1);

	combinaison(IR1);
	capteur_Infra[IR1] = ANALOG_Read(B1);

	combinaison(IR2);
	capteur_Infra[IR2] = ANALOG_Read(B1);

}

int main()
{
	// variables locales
	int j = 0;
	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");
	
	//Nouveau contenu
	int capteur_Infra[3] = {0,0,0};			// 3 capteurs infra rouge utilisé pour la grande course


	while (j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(BMP_FRONT))
		{
			LCD_Printf("Entrer dans les Tests de capteurs\n");
			j = 1;
		}
	}
	
	//Configuration
	testDeCapteurs(capteur_Infra);
	
	LCD_Printf("Fin du test du capteur\n");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}

//Debut de la fonction pour la modification des gains a suivre 
void testDeCapteurs(int capteur[3])
{
	int i = 0;
	int Affichage = 0;
	while(i==0)
	{
		lireCapteur(capteur);
		LCD_ClearAndPrint("Analog IR0 = %d\n", capteur[0]);
		LCD_Printf("Analog IR1 = %d\n", capteur[1]);
		LCD_Printf("Analog IR2 = %d\n\n", capteur[2]);
		//capteurAffichage(capteur);
		
		//Si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_RIGHT))	//Configuration 1 de la fonction de test des capteurs
		{
			i = 1;
			LCD_Printf("Sortie du test des infra\n");
		}
		THREAD_MSleep(500);
	}
}

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



