#ifndef BOUTONS_H
#define BOUTONS_H

#define LONGUEUR_CODE 5

#define BTN_0 0
#define BTN_1 1
#define BTN_2 2
#define BTN_3 3
#define BTN_4 4

//Define pour les boutons
#define ORANGE_LEFT 11
#define ORANGE_RIGHT 9
#define BMP_BLEU 1
#define BMP_ROUGE 2
#define BMP_VERT 4
#define BMP_ORANGE 3


#define BON_CODE 1

// Prototypes de fonctions de configs
int entrer_code(int code[LONGUEUR_CODE]); //Fonction pour faire entrer le code a un intrus
int nouveau_code(int code[LONGUEUR_CODE]); //Fonction pour faire choisir un nouveau code
int show_code(int code[LONGUEUR_CODE]); //Fonction pour afficher le mot de passe present
int get_bouton(); //Fonction appuyer sur bouton

#endif

