#ifndef SONAR_H
#define SONAR_H
// Include Files
#include <libarmus.h>

#define SONAR_UN 1
#define SONAR_DEUX 2
#define HYST_SONAR 4
#define HYST_LAST_SONAR 5

// Prototypes de fonctions de configs
float sonar_g();
float sonar_d();

int suivre_brigand();

#endif

