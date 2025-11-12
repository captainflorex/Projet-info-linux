/*
* ════════════════════════════════════════════════════════════════════════════
 * FICHIER: sauvegarde.h
 * Description: Module de sauvegarde - Header
 * ════════════════════════════════════════════════════════════════════════════
 */

#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include "structures.h"

#define FICHIER_SAUVEGARDE "sauvegardes.txt"

// Fonctions de sauvegarde
int sauvegarderPartie(char* pseudo, int niveau, int vies);
int chargerPartie(char* pseudo, int* niveau, int* vies);

#endif // SAUVEGARDE_H
