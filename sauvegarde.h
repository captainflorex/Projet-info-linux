#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#define FICHIER_SAUVEGARDE "sauvegardes.txt"

// Fonctions de sauvegarde
int sauvegarderPartie(char* pseudo, int niveau, int vies);

int chargerPartie(char* pseudo, int* niveau, int* vies);

typedef struct {
    char pseudo[50];
    int niveau;
    int vies;
} Enregistrement;

#endif // SAUVEGARDE_H
