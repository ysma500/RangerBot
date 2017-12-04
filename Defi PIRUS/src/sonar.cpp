#include <libarmus.h>
#include "mouvement.h"
#include "sonar.h"

int last_sonar_d = 0;
int last_sonar_g = 0;

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

	

	
int suivre_brigand()

{
	float lecture_droit = sonar_d();
	float lecture_gauche = sonar_g();
	
	if((last_sonar_d != 0) && (last_sonar_g != 0))
	{
		if(lecture_droit < lecture_gauche)	//Objet a droite
		{
			if(lecture_droit < (lecture_gauche - HYST_SONAR))
			{
				if (lecture_droit < (last_sonar_d - HYST_LAST_SONAR))
				{
					//Tourner un peu a droite et avancer un peu
					Rotation(20,RIGHT_ROT);
					Avance(MIN_DISTANCE/2, AVANCE);
				}
			}
			else
			{
				//avancer un peu
				Avance(MIN_DISTANCE/2, AVANCE);
			}
			last_sonar_d = lecture_droit;
			last_sonar_g = lecture_gauche;
			return 1;
		}
		else if(lecture_gauche < lecture_droit)	//Objet a gauche
		{
			if(lecture_gauche < (lecture_droit - HYST_SONAR))
			{
				if (lecture_gauche < (last_sonar_g - HYST_LAST_SONAR))
				{
					//Tourner un peu a gauche et avancer un peu
					Rotation(20,LEFT_ROT);
					Avance(MIN_DISTANCE/2, AVANCE);
				}
			}
			else
			{
				//avancer un peu
				Avance(MIN_DISTANCE/2, AVANCE);
			}
			last_sonar_d = lecture_droit;
			last_sonar_g = lecture_gauche;
			return 1;
		}
		else	//S'il y a rien en avant... DO NOTHING
		{
			last_sonar_d = lecture_droit;
			last_sonar_g = lecture_gauche;
			return 0;
		}
	}
}


