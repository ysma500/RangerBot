#include <libarmus.h>
#include "mouvement.h"
#include "sonar.h"

int last_sonar_d = 0;
int last_sonar_g = 0;
int flag_brigand = 0;

//Debut de la fonction pour la modification des gains a suivre 
float sonar_d()
{
	float sonar_droit = 0;
	sonar_droit = SONAR_Detect(SONAR_UN);
	LCD_Printf("Lecture sonar droit\n");
	return sonar_droit;
}

float sonar_g()
{
	float sonar_gauche = 0;
	sonar_gauche = SONAR_Detect(SONAR_DEUX);
	LCD_Printf("Lecture sonar gauche\n");
	return sonar_gauche;
}

int brigand_passif()
{
	float lecture_gauche = sonar_g();
	if (lecture_gauche < (last_sonar_g - 10))
	{
		last_sonar_g = lecture_gauche;
		return 1;
	}
	else
	{
		last_sonar_g = lecture_gauche;
		return 0;
	}
	return 0;
}

int suivre_brigand()
{
	float lecture_gauche = sonar_g();
	
	if (lecture_gauche < (last_sonar_g - HYST_LAST_SONAR))
	{
		Brake();
		last_sonar_g = lecture_gauche;
		return 1;
	}
	else
	{
		last_sonar_g = lecture_gauche;
		return 0;
	}
	return 0;
}

/*
int suivre_brigand()
{
	float lecture_droit = sonar_d();
	float lecture_gauche = sonar_g();
	
	if((last_sonar_d != 0) && (last_sonar_g != 0))	//Si ce n'est pas la premiere fois que l'on passe dans la boucle
	{
		if(lecture_droit < lecture_gauche)	//Objet le plus pret a droite
		{
			if(lecture_droit < (lecture_gauche - HYST_SONAR))	//Si l'objet est beaucoup a droite
			{
				if (lecture_droit < (last_sonar_d - HYST_LAST_SONAR))	//Si un objet s'est interposer depuis la derniere lecture
				{
					if(flag_brigand == 0)
						flag_brigand = 1;
					//Tourner un peu a droite et avancer un peu
					Rotation(20,RIGHT_ROT);
					//Avance(MIN_DISTANCE/2, AVANCE);
					LCD_Printf("Je vois un brigand a droite\n");
					last_sonar_d = lecture_droit;
					last_sonar_g = lecture_gauche;
					return 1;
				}
				else if(flag_brigand)	//Si le flag de brigand est lever
				{
					//if(lecture_gauche < MIN_DIST_BRIGAND)	//Si on est assez pres de l'individu... on arrete
					//{
					Brake();
					return 2;
					//}
					//else	//Si on est pas assez pres... On avance
					//{
					//	Avance(MIN_DISTANCE/2, AVANCE);
					//	return 1;
					//}
				}
				else	//Sinon on passe le controle aux IRs
				{
					//avancer un peu
					//LCD_Printf("Je vois un brigand 2\n");
					return 0;
				}
			}
			else	//Si l'objet est moderement a droite
			{
				if (lecture_droit < (last_sonar_d - HYST_LAST_SONAR))	//Si un objet s'est interposer depuis la derniere lecture
				{
					if(flag_brigand == 0)
						flag_brigand = 1;
					//Tourner un peu a droite et avancer un peu
					//Rotation(20,RIGHT_ROT);
					Avance(MIN_DISTANCE/2, AVANCE);
					LCD_Printf("Je vois un brigand 1 en avant\n");
					last_sonar_d = lecture_droit;
					last_sonar_g = lecture_gauche;
					return 1;
				}
				else if(flag_brigand)	//Si le flag de brigand est lever
				{
					if(lecture_gauche < MIN_DIST_BRIGAND)	//Si on est assez pres de l'individu... on arrete
					{
						Brake();
						return 2;
					}
					else	//Si on est pas assez pres... On avance
					{
						Avance(MIN_DISTANCE/2, AVANCE);
						return 1;
					}
				}
				else	//Sinon on passe le controle aux IRs
				{
					//avancer un peu
					//LCD_Printf("Je vois un brigand 2\n");
					return 0;
				}
			}
		}
		else if(lecture_gauche < lecture_droit)	//Objet a gauche plus pres
		{
			if(lecture_gauche < (lecture_droit - HYST_SONAR))	//Si l'objet est beaucoup a gauche
			{
				if (lecture_gauche < (last_sonar_g - HYST_LAST_SONAR))	//Si un objet s'est interposer depuis la derniere lecture
				{
					if(flag_brigand == 0)
						flag_brigand = 1;
					//Tourner un peu a gauche et avancer un peu
					Rotation(20,LEFT_ROT);
					//Avance(MIN_DISTANCE/2, AVANCE);
					LCD_Printf("Je vois un brigand a gauche\n");
					last_sonar_d = lecture_droit;
					last_sonar_g = lecture_gauche;
					return 1;
				}
				else if(flag_brigand)	//Si le flag de brigand est lever
				{
					if(lecture_gauche < MIN_DIST_BRIGAND)	//Si on est assez pres de l'individu... on arrete
					{
						Brake();
						return 2;
					}
					else	//Si on est pas assez pres... On avance
					{
						Avance(MIN_DISTANCE/2, AVANCE);
						return 1;
					}
				}
				else	//Sinon on passe le controle aux IRs
				{
					//avancer un peu
					//LCD_Printf("Je vois un brigand 2\n");
					return 0;
				}
			}
			else	//L'objet est moderement a gauche
			{
				if (lecture_gauche < (last_sonar_g - HYST_LAST_SONAR))	//Si un objet s'est interposer depuis la derniere lecture
				{
					if(flag_brigand == 0)
						flag_brigand = 1;
					//Tourner un peu a gauche et avancer un peu
					//Rotation(20,LEFT_ROT);
					Avance(MIN_DISTANCE/2, AVANCE);
					LCD_Printf("Je vois un brigand 2 en avant\n");
					last_sonar_d = lecture_droit;
					last_sonar_g = lecture_gauche;
					return 1;
				}
				else if(flag_brigand)	//Si le flag de brigand est lever
				{
					if(lecture_gauche < MIN_DIST_BRIGAND)	//Si on est assez pres de l'individu... on arrete
					{
						Brake();
						return 2;
					}
					else	//Si on est pas assez pres... On avance
					{
						Avance(MIN_DISTANCE/2, AVANCE);
						return 1;
					}
				}
				else	//Sinon on passe le controle aux IRs
				{
					//avancer un peu
					//LCD_Printf("Je vois un brigand 2\n");
					return 0;
				}
			}
		}
		else	//Si Sonar_droit est egal Sonar_gauche (presque impossible)
		{
			LCD_Printf("Je vois pas de brigand\n");
			last_sonar_d = lecture_droit;
			last_sonar_g = lecture_gauche;
			return 0;
		}
	}
	else	//Si c'est la premiere fois que l'on passe dans la boucle
	{
		LCD_Printf("Je vois pas de brigand\n");
		last_sonar_d = lecture_droit;
		last_sonar_g = lecture_gauche;
		return 0;
	}
}
*/

