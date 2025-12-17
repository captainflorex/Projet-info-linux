#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "moteur.h"

// === FONCTION INTERNE (Privée) ===
// Source : Assistant IA (Algorithme d'effet de zone)
// Rôle : Gère l'explosion d'une bombe (Bonus).
// Entrée : Le centre de l'explosion (X, Y).
// Sortie : Modifie le plateau et augmente le score.
static void declencherExplosionBombe(Partie* partie, int centreY, int centreX) {
    // Définit la zone de 4x4 cases
    int debutY = centreY - 1; int finY = centreY + 2;
    int debutX = centreX - 1; int finX = centreX + 2;

    for (int i = debutY; i <= finY; i++) {
        for (int j = debutX; j <= finX; j++) {
            // Vérifie qu'on ne sort pas du tableau
            if (i >= 0 && i < HAUTEUR && j >= 0 && j < LARGEUR) {
                int itemTouche = partie->tableau[i][j];
                // Si c'est un fruit, on gagne des points pour le contrat
                if (itemTouche >= 1 && itemTouche <= NB_TYPES_ITEMS) {
                    int index = itemTouche - 1;
                    if (partie->elimines[index] < partie->contrat[index]) {
                        partie->elimines[index]++;
                    }
                }
                partie->tableau[i][j] = 0; // Destruction de la case
            }
        }
    }
}

// === INITIALISATION ===
// Rôle : Prépare une nouvelle partie à zéro.
void initialiserPartie(Partie* partie) {
    partie->niveau = 1;
    partie->vies = NB_VIES_INITIAL;
    genererNiveau(partie, 1);
}

// === GÉNÉRATEUR DE NIVEAU ===
// Rôle : Configure la difficulté (Temps, Coups, Objectifs) selon le niveau choisi.
void genererNiveau(Partie* partie, int niveau) {
    partie->niveau = niveau;
    int objectifParItem = 0;

    // Configuration de la difficulté
    switch(niveau) {
        case 1: // Facile
            partie->coups_restants = 30;
            partie->temps_restant = 90;
            objectifParItem = 10;
            break;
        case 2: // Moyen
            partie->coups_restants = 35;
            partie->temps_restant = 120;
            objectifParItem = 20;
            break;
        case 3: // Difficile
            partie->coups_restants = 50;
            partie->temps_restant = 240;
            objectifParItem = 30;
            break;
        default:
            partie->coups_restants = 40;
            partie->temps_restant = 120;
            objectifParItem = 30;
            break;
    }

    // Initialisation des compteurs d'objectifs
    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        partie->contrat[i] = objectifParItem;
        partie->elimines[i] = 0;
    }
    remplirTableauAleatoire(partie);
}

// === REMPLISSAGE INITIAL ===
// Rôle : Remplit le tableau au début, en s'assurant qu'il n'y a pas déjà de matchs faits.
void remplirTableauAleatoire(Partie* partie) {
    srand(time(NULL));

    // 1. Remplissage brute
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            partie->tableau[i][j] = (rand() % NB_TYPES_ITEMS) + 1;
        }
    }

    // 2. Stabilisation (On supprime les matchs qui se sont créés par hasard)
    int iterations = 0;
    while (detecterEtEliminerMatchs(partie) > 0 && iterations < 50) {
        appliquerGravite(partie);
        remplirCasesVides(partie);
        iterations++;
    }

    // 3. Sécurité : On retire les bombes qui auraient pu apparaître au setup
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            if (partie->tableau[i][j] == ITEM_BOMBE) {
                partie->tableau[i][j] = (rand() % NB_TYPES_ITEMS) + 1;
            }
        }
    }

    // 4. Remise à zéro des scores (car la stabilisation a pu en marquer)
    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        partie->elimines[i] = 0;
    }
}

// === UTILITAIRE ADJACENCE ===
// Rôle : Vérifie mathématiquement si deux cases sont côte à côte.
// Sortie : 1 (Oui), 0 (Non).
int estAdjacent(int y1, int x1, int y2, int x2) {
    if (abs(y1 - y2) + abs(x1 - x2) == 1) return 1;
    return 0;
}

// === GESTION DU MOUVEMENT (PERMUTATION) ===
// Rôle : Tente d'échanger deux items sélectionnés par le joueur.
// Sortie : 1 si le mouvement a réussi, 0 sinon (bloqué par un mur ou non adjacent).
int permuterItems(Partie* partie, int y1, int x1, int y2, int x2) {
    if (!estAdjacent(y1, x1, y2, x2)) return 0;

    int item1 = partie->tableau[y1][x1];
    int item2 = partie->tableau[y2][x2];

    // RÈGLE IMPORTANTE : On ne peut pas déplacer un MUR
    if (item1 == ITEM_MUR || item2 == ITEM_MUR) return 0;

    // GESTION SPECIALE : Si on touche une BOMBE
    int bombeDeclenchee = 0;
    if (item1 == ITEM_BOMBE) {
        declencherExplosionBombe(partie, y1, x1);
        bombeDeclenchee = 1;
    }
    else if (item2 == ITEM_BOMBE) {
        declencherExplosionBombe(partie, y2, x2);
        bombeDeclenchee = 1;
    }

    if (bombeDeclenchee) {
        partie->coups_restants--;
        // Après explosion, on fait tomber les items
        appliquerGravite(partie);
        remplirCasesVides(partie);
        return 1; // Mouvement validé
    }

    // MOUVEMENT CLASSIQUE (Swap)
    int temp = partie->tableau[y1][x1];
    partie->tableau[y1][x1] = partie->tableau[y2][x2];
    partie->tableau[y2][x2] = temp;

    partie->coups_restants--;
    return 1;
}

// === GRAVITÉ ===
// Source : Assistant IA (Algorithme de chute avec pointeur d'écriture)
// Rôle : Fait tomber les fruits flottants vers le bas pour combler les trous.
void appliquerGravite(Partie* partie) {
    for (int j = 0; j < LARGEUR; j++) {
        int k = HAUTEUR - 1; // Pointeur d'écriture (commence en bas)
        // On parcourt la colonne de bas en haut
        for (int i = HAUTEUR - 1; i >= 0; i--) {
            if (partie->tableau[i][j] != 0) {
                // Si on trouve un fruit, on le copie à la position 'k'
                partie->tableau[k][j] = partie->tableau[i][j];
                // Si on l'a déplacé, on vide l'ancienne case
                if (i != k) partie->tableau[i][j] = 0;
                k--; // On remonte le pointeur
            }
        }
    }
}

// === REMPLISSAGE APRES CHUTE ===
// Rôle : Remplit les cases vides en haut du tableau avec de nouveaux items.
void remplirCasesVides(Partie* partie) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            if (partie->tableau[i][j] == 0) {
                int chance = rand() % 300;
                // Apparition rare de Bombe (0.3%) si niveau > 1
                if (chance == 0 && partie->niveau > 1) {
                    partie->tableau[i][j] = ITEM_BOMBE;
                }
                // Apparition rare de Mur (2%) si niveau > 1
                else if (chance >= 1 && chance <= 6 && partie->niveau > 1) {
                    partie->tableau[i][j] = ITEM_MUR;
                }
                // Sinon Fruit aléatoire
                else {
                    partie->tableau[i][j] = (rand() % NB_TYPES_ITEMS) + 1;
                }
            }
        }
    }
}

// === ALGORITHME DE DÉTECTION (Cœur du jeu) ===
// Source : Assistant IA (Algorithme de marquage matriciel complexe)
// Rôle : Scanne toute la grille pour trouver les alignements (Matchs).
// Sortie : Le nombre total d'items détruits.
int detecterEtEliminerMatchs(Partie* partie) {
    int totalElimines = 0;
    int marques[HAUTEUR][LARGEUR] = {0}; // Tableau temporaire pour noter ce qu'on va détruire

    // ÉTAPE 1 : DÉTECTION
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            int item = partie->tableau[i][j];
            // On ignore les cases vides, les bombes et les murs pour les alignements
            if (item == 0 || item == ITEM_BOMBE || item == ITEM_MUR) continue;

            // Règle : Suite de 6 (Horizontale) -> Bonus : Supprime tout le type
            if (j <= LARGEUR - 6 &&
                partie->tableau[i][j+1] == item && partie->tableau[i][j+2] == item &&
                partie->tableau[i][j+3] == item && partie->tableau[i][j+4] == item && partie->tableau[i][j+5] == item)
            {
                // On marque toutes les cases du plateau qui ont cette couleur
                for (int y = 0; y < HAUTEUR; y++)
                    for (int x = 0; x < LARGEUR; x++)
                        if (partie->tableau[y][x] == item) marques[y][x] = 1;
            }

            // Règle : Suite de 6 (Verticale)
            if (i <= HAUTEUR - 6 &&
                partie->tableau[i+1][j] == item && partie->tableau[i+2][j] == item &&
                partie->tableau[i+3][j] == item && partie->tableau[i+4][j] == item && partie->tableau[i+5][j] == item)
            {
                for (int y = 0; y < HAUTEUR; y++)
                    for (int x = 0; x < LARGEUR; x++)
                        if (partie->tableau[y][x] == item) marques[y][x] = 1;
            }

            // Règle : Croix (3x3) -> Supprime Ligne + Colonne
            if (i >= 1 && i < HAUTEUR - 1 && j >= 1 && j < LARGEUR - 1 &&
                partie->tableau[i][j-1] == item && partie->tableau[i][j+1] == item &&
                partie->tableau[i-1][j] == item && partie->tableau[i+1][j] == item)
            {
                for (int x = 0; x < LARGEUR; x++) if (partie->tableau[i][x] == item) marques[i][x] = 1;
                for (int y = 0; y < HAUTEUR; y++) if (partie->tableau[y][j] == item) marques[y][j] = 1;
            }

            // Règle : Carré (4x4) -> Supprime la zone carrée
            if (i <= HAUTEUR - 4 && j <= LARGEUR - 4) {
                int estCarre = 1;
                for (int y = i; y < i + 4; y++)
                    for (int x = j; x < j + 4; x++)
                        if (partie->tableau[y][x] != item) estCarre = 0;

                if (estCarre) {
                    for (int y = i; y < i + 4; y++)
                        for (int x = j; x < j + 4; x++) marques[y][x] = 1;
                }
            }

            // Règle de base : Suite de 4 (Horizontale)
            if (j <= LARGEUR - 4 && partie->tableau[i][j+1]==item && partie->tableau[i][j+2]==item && partie->tableau[i][j+3]==item) {
                for(int k=0; k<4; k++) marques[i][j+k] = 1;
            }
            // Règle de base : Suite de 4 (Verticale)
            if (i <= HAUTEUR - 4 && partie->tableau[i+1][j]==item && partie->tableau[i+2][j]==item && partie->tableau[i+3][j]==item) {
                for(int k=0; k<4; k++) marques[i+k][j] = 1;
            }
        }
    }

    // ÉTAPE 2 : SUPPRESSION & SCORE
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            if (marques[i][j] == 1) {
                int itemType = partie->tableau[i][j];

                // Mise à jour des objectifs (Contrat)
                if (itemType >= 1 && itemType <= NB_TYPES_ITEMS) {
                    int index = itemType - 1;
                    if (partie->elimines[index] < partie->contrat[index]) {
                        partie->elimines[index]++;
                    }
                }

                // Suppression effective
                partie->tableau[i][j] = 0;
                totalElimines++;

                // Règle : Les murs voisins sont fragilisés et détruits
                if (i > 0 && partie->tableau[i-1][j] == ITEM_MUR) partie->tableau[i-1][j] = 0;
                if (i < HAUTEUR - 1 && partie->tableau[i+1][j] == ITEM_MUR) partie->tableau[i+1][j] = 0;
                if (j > 0 && partie->tableau[i][j-1] == ITEM_MUR) partie->tableau[i][j-1] = 0;
                if (j < LARGEUR - 1 && partie->tableau[i][j+1] == ITEM_MUR) partie->tableau[i][j+1] = 0;
            }
        }
    }
    return totalElimines;
}
// === CASCADE ===
// Rôle : Répète la gravité et la détection tant qu'il y a des nouveaux matchs qui se forment.
void gererMatchsEtCascades(Partie* partie) {
    // Boucle "Jusqu'à stabilisation"
    while (detecterEtEliminerMatchs(partie) > 0) {
        appliquerGravite(partie); // Appliquer la gravité
        remplirCasesVides(partie); // Remplir les cases vides
    }
}

// === VÉRIFICATION VICTOIRE ===
// Rôle : Vérifie si tous les contrats sont remplis.
int verifierVictoire(Partie* partie) {
    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        // Si un seul objectif n'est pas atteint...
        if (partie->elimines[i] < partie->contrat[i]) {
            return 0; // Défaite (ou pas encore gagné)
        }
    }
    // Si la boucle se termine sans return 0, c'est que tout est atteint.
    return 1; // Victoire
}
