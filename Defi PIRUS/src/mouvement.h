//DEFINES
#define PI 3.14159265358979323846264338327950288
#define LEFT_MOTOR 7
#define RIGHT_MOTOR 8
#define Circum 229.33626371205490640777296697940 //Cironference de la roue
#define LEFT_ROT 1
#define RIGHT_ROT 0
#define AVANCE 2
#define RECULE 3
#define SPEED_START 70
#define DELAY_STEP 500
#define MIN_DISTANCE 200

//PID
float GAIN_I = 0.223;
float GAIN_P = 1.68;
int m_iTicTotalG = 0;
int m_iTicTotalD = 0;
int m_iFlagZoneBkp = 0;
int m_iFlagRge = 0;

//Initialisation PID
void Initialisation();

// Prototypes de fonctions (Avancer, Tourner), Fonctions de base du mouvement 
void Avance(int iDistance, int iSens);
void Rotation(float iAngle, int iDirection);
float PID_Setup(void);

//Fonctions de mouvements particuliers
void Tourne_gauche_avance();
void Tourne_gauche_avance(int valeurMod);
void Avance_BASE();