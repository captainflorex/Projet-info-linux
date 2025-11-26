#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "moteur.h"

// Initialise une nouvelle partie
void initialiserPartie(Partie* partie) {
    partie->niveau = 1;
    partie->vies = NB_VIES_INITIAL;
    genererNiveau(partie, 1);
}
 
// Génère un niveau avec une difficulté progressive et adaptée
void genererNiveau(Partie* partie, int niveau) {
    partie->niveau = niveau;

    // Configuration spécifique par niveau
    int objectifParItem = 0;

    switch(niveau) {
        case 1:
            // FACILE : On apprend à jouer
                // ~10% du plateau total à élimine
                 partie->coups_restants = 30;    // Large
        partie->temps_restant = 90;     // 1min30 (très large)
        objectifParItem = 10;           // Facile à atteindre
        break;

        case 2:
            // MOYEN : Il faut réfléchir un peu
                // On réduit les coups par rapport à l'objectif
                    partie->coups_restants = 35;    // +5 coups seulement pour double objectif
        partie->temps_restant = 120;    // 2min
        objectifParItem = 20;           // Le double du niveau 1
        break;

        case 3:
            // DIFFICILE : Nécessite des combos / figures spéciales
                partie->coups_restants = 50;
        partie->temps_restant = 240;    // 4min
        objectifParItem = 100;           // contrat
        break;

        default:
            // Sécurité pour les niveaux > 3 (si jamais)
                partie->coups_restants = 40;
        partie->temps_restant = 120;
        objectifParItem = 30;
        break;
    }
    // Initialiser le contrat
    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        partie->contrat[i] = objectifParItem;
        partie->elimines[i] = 0;
    }
    remplirTableauAleatoire(partie);
}

// Remplit le tableau aléatoirement SANS matchs de départ ET SANS BOMBES
void remplirTableauAleatoire(Partie* partie) {
    srand(time(NULL));

    // 1. Remplissage aléatoire initial
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            partie->tableau[i][j] = (rand() % NB_TYPES_ITEMS) + 1;
        }
    }

    // 2. Boucle de stabilisation
    // (Supprime les alignements de 3 créés par hasard)
    int iterations = 0;
    while (detecterEtEliminerMatchs(partie) > 0 && iterations < 50) {
        appliquerGravite(partie);
        remplirCasesVides(partie); // Ici, une bombe pourrait apparaître au niveau 2+
        iterations++;
    }

    // 3. NETTOYAGE DE SÉCURITÉ (NOUVEAU)
    // On parcourt tout le tableau une dernière fois.
    // Si on trouve une bombe (qui serait apparue pendant l'étape 2),
    // on la remplace par un fruit normal.
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            if (partie->tableau[i][j] == ITEM_BOMBE) {
                partie->tableau[i][j] = (rand() % NB_TYPES_ITEMS) + 1;
            }
        }
    }

    // 4. Réinitialiser le score
    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        partie->elimines[i] = 0;
    }
}

// Vérifie si deux cases sont adjacentes
int estAdjacent(int y1, int x1, int y2, int x2) {
    if (abs(y1 - y2) + abs(x1 - x2) == 1) {
        return 1;
    }
    return 0;
}

// Fonction pour faire exploser une zone 4x4 autour d'un point
void declencherExplosionBombe(Partie* partie, int centreY, int centreX) {
    // Une zone 4x4 signifie -1 à +2 autour du centre (ou -2 à +1)
    // On va centrer au mieux : de (y-1) à (y+2)

    int debutY = centreY - 1;
    int finY = centreY + 2;
    int debutX = centreX - 1;
    int finX = centreX + 2;

    for (int i = debutY; i <= finY; i++) {
        for (int j = debutX; j <= finX; j++) {

            // Vérification des limites du plateau
            if (i >= 0 && i < HAUTEUR && j >= 0 && j < LARGEUR) {

                int itemTouche = partie->tableau[i][j];

                // Si c'est un fruit valide, on augmente le score !
                if (itemTouche >= 1 && itemTouche <= NB_TYPES_ITEMS) {
                    int index = itemTouche - 1;
                    if (partie->elimines[index] < partie->contrat[index]) {
                        partie->elimines[index]++;
                    }
                }

                // On détruit l'item (même si c'est une autre bombe !)
                partie->tableau[i][j] = 0;
            }
        }
    }
}

int permuterItems(Partie* partie, int y1, int x1, int y2, int x2) {
    if (!estAdjacent(y1, x1, y2, x2)) return 0;

    int item1 = partie->tableau[y1][x1];
    int item2 = partie->tableau[y2][x2];

    // --- LOGIQUE MALUS : IMPOSSIBLE DE BOUGER UN MUR ---
    if (item1 == ITEM_MUR || item2 == ITEM_MUR) {
        // Astuce visuelle : on pourrait afficher un message ici
        return 0; // On refuse le mouvement
    }
    // --- LOGIQUE BOMBE ---
    int bombeDeclenchee = 0;

    // Cas 1 : L'item 1 est une bombe
    if (item1 == ITEM_BOMBE) {
        declencherExplosionBombe(partie, y1, x1);
        bombeDeclenchee = 1;
    }
    // Cas 2 : L'item 2 est une bombe
    else if (item2 == ITEM_BOMBE) {
        declencherExplosionBombe(partie, y2, x2);
        bombeDeclenchee = 1;
    }

    if (bombeDeclenchee) {
        partie->coups_restants--;

        // --- CORRECTION IMPORTANTE ---
        // On applique la gravité et le remplissage TOUT DE SUITE
        // pour boucher le trou laissé par l'explosion.
        appliquerGravite(partie);
        remplirCasesVides(partie);

        // Ensuite, le main() appellera gererMatchsEtCascades()
        // pour voir si cette chute a créé de nouveaux combos.
        return 1;
    }

    // --- PERMUTATION NORMALE (Si pas de bombe) ---

    int temp = partie->tableau[y1][x1];
    partie->tableau[y1][x1] = partie->tableau[y2][x2];
    partie->tableau[y2][x2] = temp;

    partie->coups_restants--;

    return 1;
}

void appliquerGravite(Partie* partie) {
    // Boucle pour chaque colonne
    for (int j = 0; j < LARGEUR; j++) {
        // 'k' est notre "pointeur d'écriture"
        // Il commence en bas et remonte
        int k = HAUTEUR - 1;

        // On lit la colonne de bas en haut
        for (int i = HAUTEUR - 1; i >= 0; i--) {
            // Si on trouve un item non-vide
            if (partie->tableau[i][j] != 0) {
                // On le déplace vers le bas à la position 'k'
                partie->tableau[k][j] = partie->tableau[i][j];

                // Si 'i' et 'k' ne sont pas au même endroit,
                // on vide la case d'origine
                if (i != k) {
                    partie->tableau[i][j] = 0;
                }
                // On déplace le pointeur d'écriture vers le haut
                k--;
            }
        }

        // Toutes les cases au-dessus de 'k' sont maintenant vides (0)
        // (La boucle 'i' s'est assurée de ça)
    }
}

void remplirCasesVides(Partie* partie) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            if (partie->tableau[i][j] == 0) {

                int chance = rand() % 300;

                // BOMBE (0.3%)
                if (chance == 0 && partie->niveau > 1) {
                    partie->tableau[i][j] = ITEM_BOMBE;
                }
                // MUR / MALUS (2% de chance, index 1 à 6)
                else if (chance >= 1 && chance <= 6 && partie->niveau > 1) {
                    partie->tableau[i][j] = ITEM_MUR;
                }
                // FRUIT NORMAL
                else {
                    partie->tableau[i][j] = (rand() % NB_TYPES_ITEMS) + 1;
                }
            }
        }
    }
}

int detecterEtEliminerMatchs(Partie* partie) {
    int totalElimines = 0;
    // Tableau temporaire pour marquer les items à supprimer
    int marques[HAUTEUR][LARGEUR] = {0};

    // --- 1. Détection des figures ---

    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            int item = partie->tableau[i][j];
            if (item == 0) continue; // Case vide


            // --- Règle 1 : Suite de 6 (Horizontale) ---
            // Effet : Élimine TOUS les items de ce type sur le plateau
            if (j <= LARGEUR - 6 &&
                partie->tableau[i][j+1] == item &&
                partie->tableau[i][j+2] == item &&
                partie->tableau[i][j+3] == item &&
                partie->tableau[i][j+4] == item &&
                partie->tableau[i][j+5] == item)
            {
                for (int y = 0; y < HAUTEUR; y++) {
                    for (int x = 0; x < LARGEUR; x++) {
                        if (partie->tableau[y][x] == item) {
                            marques[y][x] = 1;
                        }
                    }
                }
            }

            // --- Règle 1 : Suite de 6 (Verticale) ---
            // Effet : Élimine TOUS les items de ce type sur le plateau
            if (i <= HAUTEUR - 6 &&
                partie->tableau[i+1][j] == item &&
                partie->tableau[i+2][j] == item &&
                partie->tableau[i+3][j] == item &&
                partie->tableau[i+4][j] == item &&
                partie->tableau[i+5][j] == item)
            {
                for (int y = 0; y < HAUTEUR; y++) {
                    for (int x = 0; x < LARGEUR; x++) {
                        if (partie->tableau[y][x] == item) {
                            marques[y][x] = 1;
                        }
                    }
                }
            }

            // --- RÈGLE 2 : Croix (3x3) ---
            // Forme : Une ligne de 3H et 3V qui se croisent (Forme '+')
            // Effet : Élimine tous les items de ce type sur la LIGNE et la COLONNE
            if (i >= 1 && i < HAUTEUR - 1 && j >= 1 && j < LARGEUR - 1 &&
                // Ligne Horizontale de 3 (Gauche + Droite)
                partie->tableau[i][j-1] == item &&
                partie->tableau[i][j+1] == item &&
                // Ligne Verticale de 3 (Haut + Bas)
                partie->tableau[i-1][j] == item &&
                partie->tableau[i+1][j] == item)
            {
                // Marque toute la LIGNE (i)
                for (int x = 0; x < LARGEUR; x++) {
                    if (partie->tableau[i][x] == item) {
                        marques[i][x] = 1;
                    }
                }
                // Marque toute la COLONNE (j)
                for (int y = 0; y < HAUTEUR; y++) {
                    if (partie->tableau[y][j] == item) {
                        marques[y][j] = 1;
                    }
                }
            }

            // --- NOUVELLE RÈGLE 3 : Carré (4x4) ---
            // Effet : Élimine tous les items de ce type DANS le carré
            if (i <= HAUTEUR - 4 && j <= LARGEUR - 4) {
                int estCarre = 1;
                // Vérifie les 16 cases
                for (int y = i; y < i + 4; y++) {
                    for (int x = j; x < j + 4; x++) {
                        if (partie->tableau[y][x] != item) {
                            estCarre = 0;
                            break;
                        }
                    }
                    if (!estCarre) break;
                }

                if (estCarre) {
                    // Marque les 16 cases du carré
                    for (int y = i; y < i + 4; y++) {
                        for (int x = j; x < j + 4; x++) {
                            marques[y][x] = 1;
                        }
                    }
                }
            }

            // --- Règle 4 : Suite de 4 (Horizontale) ---
            // Effet : Élimine les 4 items
            if (j <= LARGEUR - 4 &&
                partie->tableau[i][j+1] == item &&
                partie->tableau[i][j+2] == item &&
                partie->tableau[i][j+3] == item)
            {
                marques[i][j] = 1; marques[i][j+1] = 1;
                marques[i][j+2] = 1; marques[i][j+3] = 1;
            }

            // --- Règle 4 : Suite de 4 (Verticale) ---
            // Effet : Élimine les 4 items
            if (i <= HAUTEUR - 4 &&
                partie->tableau[i+1][j] == item &&
                partie->tableau[i+2][j] == item &&
                partie->tableau[i+3][j] == item)
            {
                marques[i][j] = 1; marques[i+1][j] = 1;
                marques[i+2][j] = 1; marques[i+3][j] = 1;
            }
        }
    }

    // --- 2. Élimination, Score ET DESTRUCTION DES MURS VOISINS ---
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {

            // Si cet item doit être éliminé (c'est un fruit aligné)
            if (marques[i][j] == 1) {
                int itemType = partie->tableau[i][j];

                // 1. Score (Fruits seulement)
                if (itemType >= 1 && itemType <= NB_TYPES_ITEMS) {
                    int index = itemType - 1;
                    if (partie->elimines[index] < partie->contrat[index]) {
                        partie->elimines[index]++;
                    }
                }

                // 2. Suppression du fruit
                partie->tableau[i][j] = 0;
                totalElimines++;

                // 3. DESTRUCTION DES MURS ADJACENTS (Haut/Bas/Gauche/Droite)
                // On vérifie chaque voisin. Si c'est un MUR, on le casse (0).

                // Haut
                if (i > 0 && partie->tableau[i-1][j] == ITEM_MUR)
                    partie->tableau[i-1][j] = 0;
                // Bas
                if (i < HAUTEUR - 1 && partie->tableau[i+1][j] == ITEM_MUR)
                    partie->tableau[i+1][j] = 0;
                // Gauche
                if (j > 0 && partie->tableau[i][j-1] == ITEM_MUR)
                    partie->tableau[i][j-1] = 0;
                // Droite
                if (j < LARGEUR - 1 && partie->tableau[i][j+1] == ITEM_MUR)
                    partie->tableau[i][j+1] = 0;
            }
        }
    }
    return totalElimines;
}

void gererMatchsEtCascades(Partie* partie) {
    // Boucle "Jusqu'à stabilisation"
    while (detecterEtEliminerMatchs(partie) > 0) {
        appliquerGravite(partie);// Appliquer la gravité
        remplirCasesVides(partie);// Remplir les cases vides
    }
}

int verifierVictoire(Partie* partie) {
    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        // Si un seul objectif n'est pas atteint...
        if (partie->elimines[i] < partie->contrat[i]) {
            return 0; // Défaite
        }
    }
    // Si la boucle se termine, c'est que tout est atteint.
    return 1; // Victoire
}
