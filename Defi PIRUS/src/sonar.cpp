#include <libarmus.h>
#include "sonar.h"


//Debut de la fonction pour la modification des gains a suivre 
float sonar_d()
{
	float sonar_droit = 0;
	sonar_droit = SONAR_Detect(SONAR_UN);
	return sonar_droit;
}

float sonar_g()
{
	float sonar_gauche = 0;
	sonar_gauche = SONAR_Detect(SONAR_DEUX);
	return sonar_gauche;
}




