#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "structures.h"

// Codes couleurs pour utiliser les couleurs de Windowq
//#define COULEUR_RESET 7 windows
//#define COULEUR_ROUGE 12 windows
//#define COULEUR_VIOLET 13 windows
//#define COULEUR_ORANGE 6 windows
//#define COULEUR_VERT 10 windows
//#define COULEUR_JAUNE 14 windows
//#define COULEUR_CYAN 11 windows
//#define COULEUR_BLANC 15 windows

#define COULEUR_RESET  "\033[0m"
#define COULEUR_ROUGE  "\033[1;31m"
#define COULEUR_VIOLET "\033[1;35m"
#define COULEUR_ORANGE "\033[38;5;208m" // Supporté par la plupart des terminaux modernes
#define COULEUR_VERT   "\033[1;32m"
#define COULEUR_JAUNE  "\033[1;33m"
#define COULEUR_CYAN   "\033[1;36m"
#define COULEUR_BLANC  "\033[1;37m"

// Fonctions utilitaires d'affichage
void changerCouleur(const char* couleur); // Changé de int à char*


void portableSleep(int ms); //pour le multi os
int portableGetch(void); //pour le multi os
int portableKbhit(void); //pour le multi os

// Fonctions utilitaires d'affichage
//void changerCouleur(int couleur); windows
void allerA(int x, int y);
void effacerEcran(void);

// Affichage du menu et des écrans
void afficherMenuPrincipal(void);
void afficherRegles(void);

// Affichage du jeu
void afficherNiveauJeu(Partie* partie, int curseurX, int curseurY, int selectX, int selectY);

// Ces fonctions prennent maintenant des coordonnées
void afficherPlateau(Partie* partie, int curseurX, int curseurY, int selectX, int selectY, int startX, int startY);
void afficherStats(Partie* partie, int startX, int startY);
void afficherControles(int startX, int startY);
void afficherLegende(int startX, int startY);

//Source: Chatgpt pour la rafraichissement unique du Timer car sinon injouable
void rafraichirTimerSeulement(Partie* partie);

// Messages
void afficherVictoire(void);
void afficherEcranFinJeu(void);
void afficherDefaite(void);
void afficherMessageTemporaire(char* message);
void afficherUneCase(Partie* partie, int gridX, int gridY, int curseurX, int curseurY, int selectX, int selectY);

void saisirPseudo(char* buffer, int longueurMax);

#endif // AFFICHAGE_H
