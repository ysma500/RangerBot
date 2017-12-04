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

//Fonctions gab
int suivre_brigand()
{
	float lire_gauche= sonar_g();
	float lire_droit= sonar_d();

	return 0;
}
	
	
	
int sonar_obstacle()
{
	float lecture_droit= sonar_d;
	float lecture_gauche= sonar_g;
	
	if (lecture_droit < (last_sonar_d - hyst_sonar))
	{
		return 1;
	}
	if (lecture_gauche < (last_sonar_g -hyst_sonar))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
	last_sonar_d = lecture_droit;
	last_sonar_g = lecture_gauche;
}
	


