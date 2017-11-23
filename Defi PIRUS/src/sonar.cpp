#include <libarmus.h>
#include "master.h"


//Debut de la fonction pour la modification des gains a suivre 
int test_de_sonar()
{
	int i = 0;
	float fSonarUn = 0, fSonarDeux = 0;
	THREAD_MSleep(5000);
	while(i==0)
	{
		fSonarUn = SONAR_Detect(SONAR_UN);
		fSonarDeux = SONAR_Detect(SONAR_DEUX);
		LCD_Printf("1: %0.4fcm 2: %0.4fcm \n", fSonarUn, fSonarDeux);

		if(DIGITALIO_Read(BMP_REAR))
		{
			i = 1;
			LCD_Printf("Sortie du test\n");
		}
		THREAD_MSleep(250);
	}
	return 0;
}





