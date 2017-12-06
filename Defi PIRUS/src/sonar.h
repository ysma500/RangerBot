#ifndef SONAR_H
#define SONAR_H
// Include Files
#include <libarmus.h>

#define SONAR_UN 1
#define SONAR_DEUX 2
#define HYST_SONAR 10
#define HYST_LAST_SONAR 40
#define MIN_DIST_BRIGAND 35

extern int flag_brigand;
extern int last_sonar_d;
extern int last_sonar_g;

// Prototypes de fonctions de configs
float sonar_g();
float sonar_d();

int suivre_brigand();
int brigand_passif();

#endif

