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

// Include Files
#include <libarmus.h>

// Prototypes de fonctions de configs
void testDeCapteurs(void);

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
	testDeCapteurs();
	
	LCD_Printf("Fin du test du capteur\n");

	// Le code attent 20 secondes
	THREAD_MSleep(20000);

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}

//Debut de la fonction pour la modification des gains a suivre 
void testDeCapteurs(void)
{
	int i = 0, j = 0;
	int capteur[3] = {1,2,3};

	while(i==0 || j==0)
	{
		DIGITALIO_Write(2, 0);
		DIGITALIO_Write(3, 0);
		DIGITALIO_Write(4, 0);
		capteur[0] = ANALOG_Read(1);
		THREAD_MSleep(1500);

		DIGITALIO_Write(2, 1);
		DIGITALIO_Write(3, 0);
		DIGITALIO_Write(4, 0);
		capteur[1] = ANALOG_Read(2);
		THREAD_MSleep(1500);

		DIGITALIO_Write(2, 0);
		DIGITALIO_Write(3, 1);
		DIGITALIO_Write(4, 0);
		capteur[2] = ANALOG_Read(3);
		THREAD_MSleep(1500);



		LCD_Printf("Capteur 1 : %d\n", capteur[0]);
		LCD_Printf("Capteur 2 : %d\n", capteur[1]);
		LCD_Printf("Capteur 3 : %d\n", capteur[2]);

		//Si la "bumper switch" avant de robus est enclanchee...
		if(DIGITALIO_Read(BMP_FRONT) && DIGITALIO_Read(BMP_LEFT))	//Configuration 1 de la fonction de test des capteurs
		{

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




