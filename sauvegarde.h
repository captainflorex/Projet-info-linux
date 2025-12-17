#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#include "structures.h"

#define FICHIER_SAUVEGARDE "sauvegardes.txt"

// Sauvegarde simple (Progression générale)
int sauvegarderPartie(char* pseudo, int niveau, int vies);
int chargerPartie(char* pseudo, int* niveau, int* vies);

// NOUVEAU : Sauvegarde Complète (État exact du jeu)
int sauvegarderPartieComplete(char* pseudo, Partie* partie);
int chargerPartieComplete(char* pseudo, Partie* partie);

typedef struct {
    char pseudo[50];
    int niveau;
    int vies;
} Enregistrement;


#endif // SAUVEGARDE_H
