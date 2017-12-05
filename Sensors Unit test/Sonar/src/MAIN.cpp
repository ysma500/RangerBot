// Include Files
#include <libarmus.h>
#define SONAR_UN 1
#define SONAR_DEUX 2

int sonar_test();

int main()
{
	// variables locales
	int j = 0;

	//on choisit le bon mode de gestion d'erreur
	ERROR_SetMode(LCD_ONLY);

	// affiche sur le LCD
	LCD_ClearAndPrint("Depart du programme\n");
	LCD_Printf("Appuyer sur le bouton bleu pour commencer le programme");
	while (j == 0)
	{
		THREAD_MSleep(100);
		if(DIGITALIO_Read(1))
		{
			LCD_Printf("Depart du test des sonars\n");
			j = 1;
		}
	}

	LCD_ClearAndPrint("Appuyer sur le bouton rouge pour arreter le test\n");
	sonar_test();
	LCD_Printf("Le robot a termine le test\n");

	// On arrete tout sur la carte d'entrees/sorties
	FPGA_StopAll();

	LCD_Printf("Fin du programme\n");

	return 0;
}

//Debut de la fonction pour la modification des gains a suivre 
int sonar_test()
{
	int i = 0;
	float fSonarUn = 0, fSonarDeux = 0;
	THREAD_MSleep(5000);
	while(i==0)
	{
		fSonarUn = SONAR_Detect(SONAR_UN);
		fSonarDeux = SONAR_Detect(SONAR_DEUX);
		LCD_Printf("1: %0.4fcm 2: %0.4fcm \n", fSonarUn, fSonarDeux);

		if(DIGITALIO_Read(2))
		{
			i = 1;
			LCD_Printf("Sortie du test du capteur\n");
		}
		THREAD_MSleep(250);
	}
}



