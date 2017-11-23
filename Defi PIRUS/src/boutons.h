#define NB_CODE_MAX 5

#define RIEN 0
#define BTN_1 1
#define BTN_2 2
#define BTN_3 3
#define BTN_4 4

#define BON_CODE 1

// Prototypes de fonctions de configs
void test_des_boutons(int code[NB_CODE_MAX]);
int creer_code(int code[NB_CODE_MAX]);
int valeurBumper(void);
int trouverCode(int code[NB_CODE_MAX]);
