/*
 * ════════════════════════════════════════════════════════════════════════════
 * FICHIER: affichage.c
 * Description: Module d'affichage - Implémentation
 * ════════════════════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "affichage.h"

// Fonction pour changer la couleur du texte
void changerCouleur(int couleur) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, couleur);
}

// Fonction pour positionner le curseur
void allerA(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fonction pour effacer l'écran
void effacerEcran(void) {
    system("cls");
}

// Affiche le menu principal
void afficherMenuPrincipal(void) {
    effacerEcran();
    
    changerCouleur(COULEUR_VERT);
    printf("\n");
    printf("    ╔══════════════════════════════════════════════════════════╗\n");
    printf("    ║                                                          ║\n");
    printf("    ║            ████████  ██████ ████████                     ║\n");
    printf("    ║            ██       ██      ██                           ║\n");
    printf("    ║            ████████ ██      ████████                     ║\n");
    printf("    ║            ██       ██      ██                           ║\n");
    printf("    ║            ████████  ██████ ████████                     ║\n");
    printf("    ║                                                          ║\n");
    printf("    ║                  H E R O E S                             ║\n");
    printf("    ║                                                          ║\n");
    printf("    ╚══════════════════════════════════════════════════════════╝\n\n");
    
    changerCouleur(COULEUR_BLANC);
    printf("    ┌──────────────────────────────────────────────────────────┐\n");
    printf("    │                      MENU PRINCIPAL                      │\n");
    printf("    └──────────────────────────────────────────────────────────┘\n\n");
    
    changerCouleur(COULEUR_JAUNE);
    printf("        1. ");
    changerCouleur(COULEUR_BLANC);
    printf("Lire les règles du jeu\n\n");
    
    changerCouleur(COULEUR_JAUNE);
    printf("        2. ");
    changerCouleur(COULEUR_BLANC);
    printf("Commencer une nouvelle partie\n\n");
    
    changerCouleur(COULEUR_JAUNE);
    printf("        3. ");
    changerCouleur(COULEUR_BLANC);
    printf("Reprendre une partie sauvegardée\n\n");
    
    changerCouleur(COULEUR_JAUNE);
    printf("        4. ");
    changerCouleur(COULEUR_BLANC);
    printf("Quitter\n\n");
    
    changerCouleur(COULEUR_CYAN);
    printf("    >> Votre choix : ");
    changerCouleur(COULEUR_RESET);
}

// Affiche les règles du jeu
void afficherRegles(void) {
    effacerEcran();
    changerCouleur(COULEUR_VERT);
    printf("\n╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           RÈGLES DU JEU - ECE HEROES                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════════╝\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("═══ OBJECTIF DU JEU ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  Vous disposez d'un nombre limité de vies pour gagner tous les niveaux.\n");
    printf("  Pour gagner un niveau, vous devez remplir un CONTRAT en un temps limité\n");
    printf("  et avec un nombre maximum de coups.\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("═══ DÉROULEMENT D'UN NIVEAU ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  • Le plateau de jeu est un tableau de 25 lignes x 45 colonnes\n");
    printf("  • Au début, les cases sont remplies aléatoirement avec 5 types d'items\n");
    printf("  • Chaque niveau a un CONTRAT différent (exemple: 20 fraises, 50 oignons)\n");
    printf("  • Vous devez éliminer le nombre requis d'items en formant des figures\n");
    printf("  • Si vous perdez un niveau, vous perdez une vie\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("═══ FIGURES SPÉCIALES ET ÉLIMINATIONS ═══\n");
    changerCouleur(COULEUR_BLANC);

    changerCouleur(COULEUR_ROUGE);
    printf("  1. Suite de 6 items alignés (horizontal/vertical):\n");
    changerCouleur(COULEUR_BLANC);
    printf("     → Élimine TOUS les items du même type sur tout le plateau\n\n");

    changerCouleur(COULEUR_ROUGE);
    printf("  2. Croix de 9 items:\n");
    changerCouleur(COULEUR_BLANC);
    printf("     → Élimine tous les items du même type sur la ligne ET la colonne\n");
    printf("        (même les items isolés)\n\n");

    changerCouleur(COULEUR_ROUGE);
    printf("  3. Carré de 4x4 items identiques:\n");
    changerCouleur(COULEUR_BLANC);
    printf("     → Élimine tous les items du même type contenus dans le carré\n\n");

    changerCouleur(COULEUR_ROUGE);
    printf("  4. Suite de 4 items alignés (horizontal/vertical):\n");
    changerCouleur(COULEUR_BLANC);
    printf("     → Élimine les 4 items de la suite\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("═══ GRAVITÉ ET REMPLISSAGE ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  • Quand des items sont éliminés, les items au-dessus tombent\n");
    printf("  • Les cases vides en haut sont remplies par de nouveaux items aléatoires\n");
    printf("  • Les règles de suppression s'appliquent automatiquement jusqu'à stabilisation\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("═══ COMMENT JOUER ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  1. Déplacez-vous avec ");
    changerCouleur(COULEUR_CYAN);
    printf("Z/Q/S/D");
    changerCouleur(COULEUR_BLANC);
    printf(" ou les ");
    changerCouleur(COULEUR_CYAN);
    printf("FLÈCHES\n");
    changerCouleur(COULEUR_BLANC);
    printf("  2. Sélectionnez un item avec ");
    changerCouleur(COULEUR_CYAN);
    printf("ESPACE\n");
    changerCouleur(COULEUR_BLANC);
    printf("  3. Choisissez un item voisin (haut/bas/gauche/droite) pour permuter\n");
    printf("  4. Si la permutation forme une figure spéciale, les items sont éliminés\n");
    printf("  5. Chaque permutation compte pour 1 coup\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("═══ VICTOIRE / DÉFAITE ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  ");
    changerCouleur(COULEUR_VERT);
    printf("VICTOIRE: ");
    changerCouleur(COULEUR_BLANC);
    printf("Vous remplissez votre contrat avant la fin du temps/coups\n");
    printf("  ");
    changerCouleur(COULEUR_ROUGE);
    printf("DÉFAITE:  ");
    changerCouleur(COULEUR_BLANC);
    printf("Le temps ou les coups sont épuisés → Vous perdez 1 vie\n");
    printf("  ");
    changerCouleur(COULEUR_ROUGE);
    printf("GAME OVER:");
    changerCouleur(COULEUR_BLANC);
    printf(" Vous n'avez plus de vies\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("═══ SAUVEGARDE ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  • À la fin d'un niveau, vous pouvez sauvegarder votre progression\n");
    printf("  • Indiquez un pseudo pour enregistrer votre dernier niveau gagné\n");
    printf("  • Vous pourrez reprendre au niveau sauvegardé avec vos vies restantes\n\n");

    changerCouleur(COULEUR_CYAN);
    printf("╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║               Appuyez sur une touche pour revenir au menu...                 ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════════╝\n");
    changerCouleur(COULEUR_RESET);
}

// Affiche l'écran de jeu complet
// Affiche l'écran de jeu complet (VERSION NON-CENTRÉE QUI MARCHE)
void afficherNiveauJeu(Partie* partie, int curseurX, int curseurY, int selectX, int selectY) {
    effacerEcran();

    // En-tête
    changerCouleur(COULEUR_VERT);
    printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║");
    changerCouleur(COULEUR_JAUNE);
    // On laisse la largeur d'origine
    printf("                    ECE HEROES - NIVEAU %d                                    ", partie->niveau);
    changerCouleur(COULEUR_VERT);
    printf("║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    // Début de la zone de jeu
    printf("  ");
    changerCouleur(COULEUR_JAUNE);
    // On laisse la largeur d'origine
    printf("┌──────────────────────────────────────────────────────────┐\n");

    // Affichage du plateau
    afficherPlateau(partie, curseurX, curseurY, selectX, selectY);

    printf("  ");
    changerCouleur(COULEUR_JAUNE);
    printf("└──────────────────────────────────────────────────────────┘\n\n");

    // Affiche les stats (qui vont se placer en dur)
    afficherStats(partie);

    afficherControles();
    afficherLegende();

    changerCouleur(COULEUR_RESET);
}

// Affiche le plateau de jeu (VERSION NON-CENTRÉE QUI MARCHE)
void afficherPlateau(Partie* partie, int curseurX, int curseurY, int selectX, int selectY) {
    int tableau[HAUTEUR][LARGEUR];
    // Attention : #include <string.h> doit être en haut de affichage.c
    memcpy(tableau, partie->tableau, sizeof(partie->tableau));

    char symboles[] = {' ', 'F', 'O', 'M', 'R', 'C'};
    int couleurs[] = {COULEUR_RESET, COULEUR_ROUGE, COULEUR_VIOLET,
                      COULEUR_ORANGE, COULEUR_VIOLET, COULEUR_ORANGE};

    // Affichage simplifié (12 lignes x 20 colonnes pour la démo)
    for (int i = 0; i < 12; i++) {
        printf("  "); // Positionnement simple
        changerCouleur(COULEUR_JAUNE);
        printf("│ ");

        for (int j = 0; j < 20; j++) {
            int item = tableau[i][j];

            // Curseur actuel
            if (i == curseurY && j == curseurX) {
                changerCouleur(COULEUR_BLANC);
                printf("[");
                changerCouleur(couleurs[item]);
                printf("%c", symboles[item]);
                changerCouleur(COULEUR_BLANC);
                printf("]");
            }
            // Item sélectionné
            else if (i == selectY && j == selectX) {
                changerCouleur(COULEUR_JAUNE);
                printf("{");
                changerCouleur(couleurs[item]);
                printf("%c", symboles[item]);
                changerCouleur(COULEUR_JAUNE);
                printf("}");
            }
            // Item normal
            else {
                changerCouleur(couleurs[item]);
                printf(" %c ", symboles[item]);
            }
        }

        changerCouleur(COULEUR_JAUNE);
        printf(" │\n");
    }
}

// ... (afficherControles, afficherLegende) ...

// Affiche les statistiques (VERSION NON-CENTRÉE QUI MARCHE)
void afficherStats(Partie* partie) {
    // Positionne les stats à droite (en dur)
    int posX = 65;
    int posY = 4;

    allerA(posX, posY++);
    changerCouleur(COULEUR_JAUNE);
    printf("╔═════════════════════╗");

    allerA(posX, posY++);
    printf("║");
    changerCouleur(COULEUR_BLANC);
    printf(" VIES : ");
    changerCouleur(COULEUR_ROUGE);
    printf("%d / %d", partie->vies, NB_VIES_INITIAL);
    changerCouleur(COULEUR_JAUNE);
    printf("         ║");

    allerA(posX, posY++);
    printf("╠═════════════════════╣");

    allerA(posX, posY++);
    printf("║");
    changerCouleur(COULEUR_BLANC);
    printf(" TEMPS : ");
    changerCouleur(COULEUR_CYAN);
    printf("%02d:%02d", partie->temps_restant / 60, partie->temps_restant % 60);
    changerCouleur(COULEUR_JAUNE);
    printf("         ║");

    allerA(posX, posY++);
    printf("║");
    changerCouleur(COULEUR_BLANC);
    printf(" COUPS : ");
    changerCouleur(COULEUR_CYAN);
    printf("%-3d", partie->coups_restants);
    changerCouleur(COULEUR_JAUNE);
    printf("         ║");

    allerA(posX, posY++);
    printf("╠═════════════════════╣");

    allerA(posX, posY++);
    printf("║");
    changerCouleur(COULEUR_VERT);
    printf("     CONTRAT     ");
    changerCouleur(COULEUR_JAUNE);
    printf("║");

    allerA(posX, posY++);
    printf("╠═════════════════════╣");

    // Symboles (doivent correspondre à afficherPlateau)
    char symboles[] = {' ', 'F', 'O', 'M', 'R', 'C'};

    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        allerA(posX, posY++);
        printf("║   ");
        changerCouleur(COULEUR_BLANC);

        printf("[%c]  %3d / %-3d",
               symboles[i+1],
               partie->elimines[i],
               partie->contrat[i]);

        changerCouleur(COULEUR_JAUNE);
        printf("     ║");
    }

    allerA(posX, posY++);
    printf("╚═════════════════════╝");

    changerCouleur(COULEUR_RESET);
}

// Affiche les contrôles
void afficherControles(void) {
    changerCouleur(COULEUR_VERT);
    printf("  ╔════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("  ║ ");
    changerCouleur(COULEUR_JAUNE);
    printf("CONTRÔLES");
    changerCouleur(COULEUR_VERT);
    printf("                                                                  ║\n");
    printf("  ║ ");
    changerCouleur(COULEUR_BLANC);
    printf("Déplacement: ");
    changerCouleur(COULEUR_CYAN);
    printf("[Z/Q/S/D]");
    changerCouleur(COULEUR_BLANC);
    printf(" ou ");
    changerCouleur(COULEUR_CYAN);
    printf("[Flèches]");
    changerCouleur(COULEUR_BLANC);
    printf("  │  Sélection: ");
    changerCouleur(COULEUR_CYAN);
    printf("[ESPACE]");
    changerCouleur(COULEUR_BLANC);
    printf("  │  Quitter: ");
    changerCouleur(COULEUR_CYAN);
    printf("[ECHAP]");
    changerCouleur(COULEUR_VERT);
    printf("  ║\n");
    printf("  ╚════════════════════════════════════════════════════════════════════════════════════╝\n\n");
}

// Affiche la légende
void afficherLegende(void) {
    changerCouleur(COULEUR_BLANC);
    printf("  Légende: ");
    changerCouleur(COULEUR_ROUGE);
    printf("[F]");
    changerCouleur(COULEUR_BLANC);
    printf("=Fraise  ");
    changerCouleur(COULEUR_VIOLET);
    printf("[O]");
    changerCouleur(COULEUR_BLANC);
    printf("=Oignon  ");
    changerCouleur(COULEUR_ORANGE);
    printf("[M]");
    changerCouleur(COULEUR_BLANC);
    printf("=Mandarine  ");
    changerCouleur(COULEUR_VIOLET);
    printf("[R]");
    changerCouleur(COULEUR_BLANC);
    printf("=Raisin  ");
    changerCouleur(COULEUR_ORANGE);
    printf("[C]");
    changerCouleur(COULEUR_BLANC);
    printf("=Carotte\n");
}
// Affiche un message de victoire
void afficherVictoire(void) {
    changerCouleur(COULEUR_VERT);
    printf("\n\n");
    printf("    ╔══════════════════════════════════════════════════════════╗\n");
    printf("    ║                  NIVEAU RÉUSSI !                         ║\n");
    printf("    ╚══════════════════════════════════════════════════════════╝\n");
    changerCouleur(COULEUR_RESET);
}

// Affiche un message de défaite
void afficherDefaite(void) {
    changerCouleur(COULEUR_ROUGE);
    printf("\n\n");
    printf("    ╔══════════════════════════════════════════════════════════╗\n");
    printf("    ║                  NIVEAU ÉCHOUÉ                           ║\n");
    printf("    ╚══════════════════════════════════════════════════════════╝\n");
    changerCouleur(COULEUR_RESET);
}

// Affiche un message temporaire
void afficherMessageTemporaire(char* message) {
    changerCouleur(COULEUR_JAUNE);
    printf("\n  >> %s\n", message);
    changerCouleur(COULEUR_RESET);
}
