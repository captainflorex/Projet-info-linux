/*
* ════════════════════════════════════════════════════════════════════════════
 * FICHIER: sauvegarde.c
 * Description: Module de sauvegarde - Implémentation
 * ════════════════════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <string.h>
#include "sauvegarde.h"

// Sauvegarde une partie
int sauvegarderPartie(char* pseudo, int niveau, int vies) {
    FILE* fichier = fopen(FICHIER_SAUVEGARDE, "a");
    if (fichier == NULL) {
        return 0;
    }

    fprintf(fichier, "%s %d %d\n", pseudo, niveau, vies);
    fclose(fichier);
    return 1;
}

// Charge une partie
int chargerPartie(char* pseudo, int* niveau, int* vies) {
    FILE* fichier = fopen(FICHIER_SAUVEGARDE, "r");
    if (fichier == NULL) {
        return 0;
    }

    char pseudoLu[50];
    int niveauLu, viesLu;

    while (fscanf(fichier, "%s %d %d", pseudoLu, &niveauLu, &viesLu) == 3) {
        if (strcmp(pseudoLu, pseudo) == 0) {
            *niveau = niveauLu;
            *vies = viesLu;
            fclose(fichier);
            return 1;
        }
    }

    fclose(fichier);
    return 0;
}
