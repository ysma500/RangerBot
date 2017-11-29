#include <libarmus.h>
#include "sonar.h"


//Debut de la fonction pour la modification des gains a suivre 
int sonar_d()
{
	float sonar_droit = 0;
	sonar_droit = SONAR_Detect(SONAR_UN);
	LCD_Printf("%0.4fcm \n", sonar_droit);
	THREAD_MSleep(250);
	return sonar_droit;
}

int sonar_g()
{
	sonar_gauche = 0;
	sonar_gauche = SONAR_Detect(SONAR_DEUX);
	LCD_Printf("%0.4fcm \n", sonar_gauche);
	THREAD_MSleep(250);
	return sonar_gauche;
}




