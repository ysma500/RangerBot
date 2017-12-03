#define NB_CODE_MAX 5

#define RIEN 0
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
void test_des_boutons(int code[NB_CODE_MAX]);
int creer_code(int code[NB_CODE_MAX]);
int valeurBumper(void);
int trouverCode(int code[NB_CODE_MAX]);
