// Include Files
#include <libarmus.h>
#include "boutons.h"

int entrer_code(int code[LONGUEUR_CODE])
{
	int verification;
	int code_entre;
	LCD_ClearAndPrint("Entrer le code du robot pour desactiver l'alarme!\n");
	for (verification = 0; code[verification]; verification++)
	{
		code_entre = get_bouton();
		if (code_entre != code[verification])
		{
			return 0;
		}
	}
	return 1;
}

int nouveau_code(int code[LONGUEUR_CODE])
{
	int i, j;
	for (i = 0; i < LONGUEUR_CODE; i++)
	{
		code[i] = get_bouton();
		THREAD_MSleep(700);
	}
	
	return 0;
}

int show_code(int code[LONGUEUR_CODE])
{
	int i;
	LCD_ClearAndPrint("Le code est presentement : \n");
	for (i = 0; i < LONGUEUR_CODE; i++)
	{
		switch (code[i])
		{
			case BMP_BLEU : 
				LCD_Printf(" [BLEU] ");
				break;
			case BMP_VERT :
				LCD_Printf(" [VERT] ");
				break;
			case BMP_ROUGE : 
				LCD_Printf(" [ROUGE] ");
				break;
			case BMP_ORANGE : 
				LCD_Printf(" [ORANGE] ");
				break;
		}
	}
	THREAD_MSleep(3000);
	return 0;
}

//TODO logique si rien n'est pressé pour la création d'un code....
int get_bouton()
{
	int i = 0;
	LCD_Printf("Appuyer sur un bouton \n");
	while (i == 0)
	{
		if(DIGITALIO_Read(BMP_BLEU))
		{
			return BMP_BLEU;
		}
		else if(DIGITALIO_Read(BMP_ROUGE))
		{
			return BMP_ROUGE;
		}
		else if(DIGITALIO_Read(BMP_VERT))
		{
			return BMP_VERT;
		}
		else if(DIGITALIO_Read(BMP_ORANGE))
		{
			return BMP_ORANGE;
		}
		else if (DIGITALIO_Read(ORANGE_LEFT))
		{
			i = 0;
		}
		THREAD_MSleep(100);
	}
	THREAD_MSleep(2000);
	return 0;
}

