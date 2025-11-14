#ifndef MOTEUR_H
#define MOTEUR_H

#include "structures.h"

// Initialisation
void initialiserPartie(Partie* partie);

void genererNiveau(Partie* partie, int niveau);
void gererMatchsEtCascades(Partie* partie);

void remplirTableauAleatoire(int tableau[HAUTEUR][LARGEUR]);
void remplirCasesVides(Partie* partie);
void appliquerGravite(Partie* partie);

int permuterItems(Partie* partie, int y1, int x1, int y2, int x2);
int estAdjacent(int y1, int x1, int y2, int x2);
int verifierVictoire(Partie* partie);
int detecterEtEliminerMatchs(Partie* partie);

#endif // MOTEUR_H
