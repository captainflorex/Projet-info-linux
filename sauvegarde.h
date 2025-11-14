#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#define FICHIER_SAUVEGARDE "sauvegardes.txt"

// Fonctions de sauvegarde
int sauvegarderPartie(char* pseudo, int niveau, int vies);

int chargerPartie(char* pseudo, int* niveau, int* vies);

#endif // SAUVEGARDE_H
