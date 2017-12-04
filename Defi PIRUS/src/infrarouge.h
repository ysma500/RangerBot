#ifndef INFRAROUGE_H
#define INFRAROUGE_H
//Define
#define IR0 0	//Capteur plus bas
#define IR1 1	//Capteur niveau moyen
#define IR2 2	//Capteur plus haut
#define B1 1 //Analog input 1 à l'endroit B1
#define DISTANCE_MIN 100 //What is this

// Prototypes de fonctions de configs
void afficher_IR(int capteur[3]);
//int capteurAffichage(int capteur[3]);

int combinaison(int capteur);
int lireCapteur(int* capteur_Infra[3]);

#endif

