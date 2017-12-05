#ifndef MOUVEMENT_H
#define MOUVEMENT_H

//DEFINES
#define LEFT_MOTOR 7
#define RIGHT_MOTOR 8
#define MOTOR_GAUCHE 7
#define MOTOR_DROIT 8

#define PI 3.14159265358979323846264338327950288
#define Circum 229.33626371205490640777296697940 //Circonference de la roue

#define LEFT_ROT 1
#define RIGHT_ROT 0
#define AVANCE 2
#define RECULE 3

#define SPEED_START 70
#define DELAY_STEP 500
#define MIN_DISTANCE 75

#define HYST_MOV 25

//Define de mouvement pour infra PIRUS
#define DEVANT_MOV 0
#define DROIT_MOV 1
#define GAUCHE_MOV 2

//Initialisation PID
void Initialisation();

// Prototypes de fonctions (Avancer, Tourner), Fonctions de base du mouvement
void Avance(int iDistance, int iSens);
void Rotation(float iAngle, int iDirection);
float PID_Setup(void);
void Brake();

//Fonctions de mouvements particuliers

void Mouv_infra();

#endif
