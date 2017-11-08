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

//Detecter le son de 5kHz
//ANALOG_Read()
//Two inputs are : AN_IN7, AN_IN6
void testDeCapteurs(void)
{
	int condition = 0;
	int micro_sound;
	int micro_background;
	int micro_result;
	while (condition == 0)
	{
		micro_sound = ANALOG_Read(6); ////
		micro_background = ANALOG_Read(7); ////
		micro_result = micro_sound - micro_background;
		LCD_Printf("Micro 5kHz filtre : %d \n", micro_sound);
		LCD_Printf("Micro background : %d \n", micro_background);
		LCD_Printf("Micro resultat : %d \n", micro_result);
		THREAD_MSleep(100);
		if (micro_result >= 3) 
		{
			LCD_Printf("Le signal de 5kHz a ete entendu \n");
			condition = 1;
		}
		else 
		{
			LCD_Printf("Aucune reception du son\n");
			THREAD_MSleep(1000);
		}
		
	}
	
}

//ANALOG_Read()



