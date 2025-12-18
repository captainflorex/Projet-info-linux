#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

#include "affichage.h"

// Définition statique des symboles pour ne pas surcharger la mémoire
// Source : Assistant IA (Optimisation mémoire)
static const char* SYMBOLES[] = {" ", "🍓", "🧅", "🍊", "🍇", "🥕", "💣", "🧱", "🦠"};
static const char*  COULEURS_ITEMS[] = {
    COULEUR_RESET,
    COULEUR_ROUGE, COULEUR_VIOLET, COULEUR_ORANGE, COULEUR_VIOLET, COULEUR_ORANGE,
    COULEUR_BLANC, // Bombe
    COULEUR_BLANC,  // Mur
    COULEUR_VERT   // Virus
};

// === FONCTIONS UTILITAIRES WINDOWS ===
// Source : Documentation Windows.h / Assistant IA
// Permet de manipuler la console (Couleurs, Position du curseur).
/*
void changerCouleur(int couleur) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, couleur);
}

void allerA(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void effacerEcran(void) {
    system("cls");
}

*/

void changerCouleur(const char* couleur) {
    printf("%s", couleur);
}

void allerA(int x, int y) {
    // ANSI utilise 1,1 comme origine, Windows 0,0. On ajoute +1.
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

void effacerEcran(void) {
    printf("\033[H\033[J"); // Code ANSI pour effacer
    fflush(stdout);
}

void portableSleep(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000); // usleep utilise des microsecondes
#endif
}

// Implémentation de getch() pour Linux (via termios)
int portableGetch(void) {
#ifdef _WIN32
    return getch();
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// Implémentation de kbhit() pour Linux
int portableKbhit(void) {
#ifdef _WIN32
    return kbhit();
#else
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
#endif
}

// === AFFICHAGE DES MENUS ===

void afficherMenuPrincipal(void) {
    effacerEcran();

    changerCouleur(COULEUR_VERT);
    printf("\n");
    printf("    ╔══════════════════════════════════════════════════════════╗\n");
    printf("    ║                                                          ║\n");
    printf("    ║               ████████  ███████ ████████                 ║\n");
    printf("    ║               ██       ██       ██                       ║\n");
    printf("    ║               ███████  ██       ███████                  ║\n");
    printf("    ║               ██       ██       ██                       ║\n");
    printf("    ║               ████████  ███████ ████████                 ║\n");
    printf("    ║                                                          ║\n");
    printf("    ║                       H E R O E S                        ║\n");
    printf("    ║                                                          ║\n");
    printf("    ╚══════════════════════════════════════════════════════════╝\n\n");

    changerCouleur(COULEUR_BLANC);
    printf("    ┌──────────────────────────────────────────────────────────┐\n");
    printf("    │                      MENU PRINCIPAL                      │\n");
    printf("    └──────────────────────────────────────────────────────────┘\n\n");

    changerCouleur(COULEUR_JAUNE); printf("        1. ");
    changerCouleur(COULEUR_BLANC); printf("Lire les règles du jeu\n\n");
    changerCouleur(COULEUR_JAUNE); printf("        2. ");
    changerCouleur(COULEUR_BLANC); printf("Commencer une nouvelle partie\n\n");
    changerCouleur(COULEUR_JAUNE); printf("        3. ");
    changerCouleur(COULEUR_BLANC); printf("Reprendre une partie sauvegardée\n\n");
    changerCouleur(COULEUR_JAUNE); printf("        4. ");
    changerCouleur(COULEUR_BLANC); printf("Quitter\n\n");
    changerCouleur(COULEUR_CYAN);  printf("    >> Votre choix : ");
    changerCouleur(COULEUR_RESET);
}

// === AFFICHAGE DES RÈGLES (Texte Original) ===
void afficherRegles(void) {
    effacerEcran();
    changerCouleur(COULEUR_VERT);
    printf("\n╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           RÈGLES DU JEU - ECE HEROES                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════════╝\n\n");

    changerCouleur(COULEUR_JAUNE); printf("═══ OBJECTIF DU JEU ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  Vous disposez d'un nombre limité de vies pour gagner tous les niveaux.\n");
    printf("  Pour gagner un niveau, vous devez remplir un CONTRAT en un temps limité\n");
    printf("  et avec un nombre maximum de coups.\n\n");

    changerCouleur(COULEUR_JAUNE); printf("═══ DÉROULEMENT D'UN NIVEAU ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  • Le plateau de jeu est un tableau de 20 lignes x 15 colonnes (le plateau 25x45 n'est pas adapté)\n");
    printf("  • Au début, les cases sont remplies aléatoirement avec 5 types d'items\n");
    printf("  • Chaque niveau a un CONTRAT différent (exemple: 20 fraises, 50 oignons)\n");
    printf("  • Vous devez éliminer le nombre requis d'items en formant des figures\n");
    printf("  • Si vous perdez un niveau, vous perdez une vie\n\n");

    changerCouleur(COULEUR_JAUNE); printf("═══ FIGURES SPÉCIALES ET ÉLIMINATIONS ═══\n");
    changerCouleur(COULEUR_BLANC);

    changerCouleur(COULEUR_ROUGE); printf("  1. Suite de 6 items alignés (horizontal/vertical):\n");
    changerCouleur(COULEUR_BLANC); printf("     → Élimine TOUS les items du même type sur tout le plateau\n\n");

    changerCouleur(COULEUR_ROUGE); printf("  2. Croix de 9 items:\n");
    changerCouleur(COULEUR_BLANC); printf("     → Élimine tous les items du même type sur la ligne ET la colonne\n");
    printf("        (même les items isolés)\n\n");

    changerCouleur(COULEUR_ROUGE); printf("  3. Carré de 4x4 items identiques:\n");
    changerCouleur(COULEUR_BLANC); printf("     → Élimine tous les items du même type contenus dans le carré\n\n");

    changerCouleur(COULEUR_ROUGE); printf("  4. Suite de 4 items alignés (horizontal/vertical):\n");
    changerCouleur(COULEUR_BLANC); printf("     → Élimine les 4 items de la suite\n\n");

    changerCouleur(COULEUR_JAUNE); printf("═══ GRAVITÉ ET REMPLISSAGE ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  • Quand des items sont éliminés, les items au-dessus tombent\n");
    printf("  • Les cases vides en haut sont remplies par de nouveaux items aléatoires\n");
    printf("  • Les règles de suppression s'appliquent automatiquement jusqu'à stabilisation\n\n");

    changerCouleur(COULEUR_JAUNE); printf("═══ COMMENT JOUER ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  1. Déplacez-vous avec "); changerCouleur(COULEUR_CYAN); printf("Z/Q/S/D"); changerCouleur(COULEUR_BLANC);
    printf(" ou les "); changerCouleur(COULEUR_CYAN); printf("FLÈCHES\n"); changerCouleur(COULEUR_BLANC);
    printf("  2. Sélectionnez un item avec "); changerCouleur(COULEUR_CYAN); printf("ESPACE\n"); changerCouleur(COULEUR_BLANC);
    printf("  3. Choisissez un item voisin (haut/bas/gauche/droite) pour permuter\n");
    printf("  4. Si la permutation forme une figure spéciale, les items sont éliminés\n");
    printf("  5. Chaque permutation compte pour 1 coup\n\n");

    changerCouleur(COULEUR_JAUNE); printf("═══ VICTOIRE / DÉFAITE ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  "); changerCouleur(COULEUR_VERT); printf("VICTOIRE: "); changerCouleur(COULEUR_BLANC);
    printf("Vous remplissez votre contrat avant la fin du temps/coups\n");
    printf("  "); changerCouleur(COULEUR_ROUGE); printf("DÉFAITE:  "); changerCouleur(COULEUR_BLANC);
    printf("Le temps ou les coups sont épuisés → Vous perdez 1 vie\n");
    printf("  "); changerCouleur(COULEUR_ROUGE); printf("GAME OVER:"); changerCouleur(COULEUR_BLANC);
    printf(" Vous n'avez plus de vies\n\n");

    changerCouleur(COULEUR_JAUNE); printf("═══ SAUVEGARDE ═══\n");
    changerCouleur(COULEUR_BLANC);
    printf("  • À la fin d'un niveau, vous pouvez sauvegarder votre progression\n");
    printf("  • Indiquez un pseudo pour enregistrer votre dernier niveau gagné\n");
    printf("  • Vous pourrez reprendre au niveau sauvegardé avec vos vies restantes\n\n");

    changerCouleur(COULEUR_CYAN);
    printf("╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║               Appuyez sur une touche pour revenir au menu...                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════════╝\n");
    changerCouleur(COULEUR_RESET);
}

// === AFFICHAGE DU JEU (HUB) ===
// Rôle : Fonction principale qui appelle toutes les sous-fonctions d'affichage (Plateau, Stats, etc.)
void afficherNiveauJeu(Partie* partie, int curseurX, int curseurY, int selectX, int selectY) {
    effacerEcran();

    // 1. Titre
    changerCouleur(COULEUR_JAUNE);
    printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                ECE HEROES - NIVEAU %d                               ║\n", partie->niveau);
    printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    // 2. Calcul des positions
    int plateauX = 3;
    int plateauY = 5;

    int largeurPlateau = (LARGEUR * 4) + 2;
    int hauteurPlateau = HAUTEUR + 2;

    int statsX = plateauX + largeurPlateau + 3;
    int statsY = plateauY;
    int controlesY = plateauY + hauteurPlateau + 2;
    int controlesX = plateauX;
    int legendeY = controlesY + 5;
    int legendeX = plateauX;

    // 3. Dessin des composants
    afficherPlateau(partie, curseurX, curseurY, selectX, selectY, plateauX, plateauY);
    afficherStats(partie, statsX, statsY);
    afficherControles(controlesX, controlesY);
    afficherLegende(legendeX, legendeY);

    changerCouleur(COULEUR_RESET);
}

// === AFFICHAGE DE LA GRILLE (PLATEAU) ===
// Rôle : Dessine le tableau avec les fruits, les bordures et gère le curseur.
// Note : Les bordures sont forcées en JAUNE fixe.
void afficherPlateau(Partie* partie, int curseurX, int curseurY, int selectX, int selectY, int startX, int startY) {
    int largeurCase = 4;

    // --- DESSIN DES BORDURES EXTÉRIEURES ---
    changerCouleur(COULEUR_JAUNE);

    // Cadre Haut
    allerA(startX, startY);
    printf("╔");
    for (int j = 0; j < (LARGEUR * largeurCase); j++) printf("═");
    printf("╗");

    // Côtés (Gauche et Droite)
    for (int i = 0; i < HAUTEUR; i++) {
        allerA(startX, startY + 1 + i); printf("║");
        allerA(startX + (LARGEUR * largeurCase) + 1, startY + 1 + i); printf("║");
    }

    // Cadre Bas
    allerA(startX, startY + HAUTEUR + 1);
    printf("╚");
    for (int j = 0; j < (LARGEUR * largeurCase); j++) printf("═");
    printf("╝");

    // --- DESSIN DU CONTENU (FRUITS) ---
    for (int i = 0; i < HAUTEUR; i++) {
        allerA(startX + 1, startY + 1 + i);

        for (int j = 0; j < LARGEUR; j++) {
            int item = partie->tableau[i][j];

            // Gestion visuelle du curseur [ ] et de la sélection { }
            if (i == curseurY && j == curseurX) {
                changerCouleur(COULEUR_BLANC); printf("[");
                changerCouleur(COULEURS_ITEMS[item]); printf("%-2s", SYMBOLES[item]);
                changerCouleur(COULEUR_BLANC); printf("]");
            }
            else if (i == selectY && j == selectX) {
                changerCouleur(COULEUR_JAUNE); printf("{");
                changerCouleur(COULEURS_ITEMS[item]); printf("%-2s", SYMBOLES[item]);
                changerCouleur(COULEUR_JAUNE); printf("}");
            }
            else {
                changerCouleur(COULEURS_ITEMS[item]);
                printf(" %-2s ", SYMBOLES[item]);
            }
        }
    }
    // On remet la couleur normale après avoir dessiné
    changerCouleur(COULEUR_RESET);
}

// === AFFICHAGE DES STATISTIQUES (HUD) ===
void afficherStats(Partie* partie, int startX, int startY) {
    int posX = startX;
    int posY = startY;

    allerA(posX, posY++);
    changerCouleur(COULEUR_JAUNE); printf("╔═════════════════════╗");

    allerA(posX, posY++);
    printf("║"); changerCouleur(COULEUR_BLANC); printf(" VIES : ");
    changerCouleur(COULEUR_ROUGE); printf("%d / %d", partie->vies, NB_VIES_INITIAL);
    changerCouleur(COULEUR_JAUNE); printf("        ║");

    allerA(posX, posY++);
    printf("║"); changerCouleur(COULEUR_BLANC); printf(" TEMPS : ");
    changerCouleur(COULEUR_CYAN);  printf("%02d:%02d", partie->temps_restant / 60, partie->temps_restant % 60);
    changerCouleur(COULEUR_JAUNE); printf("       ║");

    allerA(posX, posY++);
    printf("║"); changerCouleur(COULEUR_BLANC); printf(" COUPS : ");
    changerCouleur(COULEUR_CYAN);  printf("%-3d", partie->coups_restants);
    changerCouleur(COULEUR_JAUNE); printf("         ║");

    allerA(posX, posY++); printf("╠═════════════════════╣");
    allerA(posX, posY++);
    printf("║"); changerCouleur(COULEUR_VERT); printf("       CONTRAT     "); changerCouleur(COULEUR_JAUNE); printf("  ║");
    allerA(posX, posY++); printf("╠═════════════════════╣");

    // Affichage dynamique des contrats (Objectifs)
    for (int i = 0; i < NB_TYPES_ITEMS; i++) {
        allerA(posX, posY++);
        printf("║   ");
        changerCouleur(COULEUR_BLANC);
        printf("[%s]  %3d / %-3d", SYMBOLES[i+1], partie->elimines[i], partie->contrat[i]);
        changerCouleur(COULEUR_JAUNE); printf("   ║");
    }

    allerA(posX, posY++); printf("╚═════════════════════╝");
    changerCouleur(COULEUR_RESET);
}

// === AFFICHAGE DES CONTRÔLES (Bas de page) ===
void afficherControles(int startX, int startY) {
    int X = startX;
    int Y = startY;
    int W = 75; // Largeur de la boîte

    changerCouleur(COULEUR_VERT);
    allerA(X, Y++); printf("╔"); for (int j = 0; j < W - 2; j++) printf("═"); printf("╗");
    allerA(X, Y++); printf("║ ");

    changerCouleur(COULEUR_JAUNE); printf("CONTRÔLES"); changerCouleur(COULEUR_VERT);
    allerA(X + W - 1, Y - 1); printf("║");

    allerA(X, Y++); printf("║ ");
    changerCouleur(COULEUR_BLANC); printf("Déplacement: "); changerCouleur(COULEUR_CYAN); printf("[Z/Q/S/D]");
    changerCouleur(COULEUR_BLANC); printf(" | Sélection: "); changerCouleur(COULEUR_CYAN); printf("[ESPACE]");
    changerCouleur(COULEUR_BLANC); printf(" | Quitter: "); changerCouleur(COULEUR_CYAN); printf("[ECHAP]");

    changerCouleur(COULEUR_VERT);
    allerA(X + W - 1, Y - 1); printf("║");

    allerA(X, Y++); printf("╚"); for (int j = 0; j < W - 2; j++) printf("═"); printf("╝");
}

void afficherLegende(int startX, int startY) {
    allerA(startX, startY);
    changerCouleur(COULEUR_BLANC); printf("Légende: ");
    changerCouleur(COULEUR_ROUGE); printf("[🍓]=Fraise  ");
    changerCouleur(COULEUR_JAUNE); printf("[🧅]=Oignon  ");
    changerCouleur(COULEUR_ORANGE); printf("[🍊]=Mandarine  ");
    changerCouleur(COULEUR_VIOLET); printf("[🍇]=Raisin  ");
    changerCouleur(COULEUR_ORANGE); printf("[🥕]=Carotte");
}

void afficherVictoire(void) {
    effacerEcran();
    int posX = 20; int posY = 5;
    changerCouleur(COULEUR_JAUNE);
    allerA(posX, posY++); printf("             ___________");
    allerA(posX, posY++); printf("            '._==_==_=_.'");
    allerA(posX, posY++); printf("            .-\\:      /-.");
    allerA(posX, posY++); printf("           | (|:.     |) |");
    allerA(posX, posY++); printf("            '-|:.     |-'");

    int textY = posY + 2;
    for (int i = 0; i < 6; i++) {
        allerA(posX - 5, textY);
        if (i % 2 == 0) changerCouleur(COULEUR_VERT); else changerCouleur(COULEUR_BLANC);
        printf("    >>>   N I V E A U   R É U S S I   <<<    ");
        portableSleep(150);
    }
    changerCouleur(COULEUR_CYAN);
    allerA(posX - 2, textY + 3);
    printf("Appuyez sur une touche pour continuer...");
    changerCouleur(COULEUR_RESET);
}

void afficherDefaite(void) {
    effacerEcran();
    int posX = 20; int posY = 5;
    changerCouleur(COULEUR_BLANC);
    allerA(posX, posY++); printf("      _.--\"\"--._");
    allerA(posX, posY++); printf("     /  _    _  \\");
    allerA(posX, posY++); printf("  _  ( (0)  (0) )  _");

    int textY = posY + 3;
    for (int i = 0; i < 6; i++) {
        allerA(posX - 2, textY);
        changerCouleur(COULEUR_ROUGE);
        printf("    !!!   G A M E   O V E R   !!!    ");
        portableSleep(150);
    }
    changerCouleur(COULEUR_CYAN);
    allerA(posX, textY + 3);
    printf("Appuyez sur une touche...");
    changerCouleur(COULEUR_RESET);
}

void afficherEcranFinJeu(void) {
    effacerEcran();
    int posX = 40; int posY = 5;
    const char* couleurs[] = {COULEUR_JAUNE, COULEUR_ORANGE, COULEUR_ROUGE, COULEUR_VIOLET, COULEUR_CYAN, COULEUR_VERT};
    int frame = 0;

    // Animation finale
    while (!portableKbhit()) {
        changerCouleur(couleurs[frame % 6]);
        int y = posY;
        allerA(posX, y++); printf("   ___  _  _   _   __  __  ___  ___   ___  _  _  ");
        allerA(posX, y++); printf("  / __|| || | /_\\ |  \\/  || _ \\|_ _| / _ \\| \\| | ");
        allerA(posX, y++); printf(" | (__ | __ |/ _ \\| |\\/| ||  _/ | |   (_) | .` | ");
        allerA(posX, y++); printf("  \\___||_||_/_/ \\_\\_|  |_||_|  |___| \\___/|_|\\_| ");

        changerCouleur(COULEUR_CYAN);
        allerA(posX + 5, y + 4);
        printf(">>> APPUYEZ SUR UNE TOUCHE POUR QUITTER <<<");
        portableSleep(150);
        frame++;
    }
    portableGetch();
    changerCouleur(COULEUR_RESET);
    effacerEcran();
}

void afficherMessageTemporaire(char* message) {
    changerCouleur(COULEUR_JAUNE);
    printf("\n  >> %s\n", message);
    changerCouleur(COULEUR_RESET);
}

// Source : Assistant IA (Algorithme d'optimisation)
// Rôle : Ne rafraîchit que les chiffres du temps pour éviter le scintillement de l'écran.
void rafraichirTimerSeulement(Partie* partie) {
    int largeurPlateau = (LARGEUR * 4) + 2;
    int statsX = 3 + largeurPlateau + 3;

    // Position exacte des chiffres du Timer
    allerA(statsX + 11, 7);
    changerCouleur(COULEUR_CYAN);
    printf("%01d:%01d", partie->temps_restant / 60, partie->temps_restant % 60);
    allerA(0, 0);
}

// === DESSINER UNE CASE UNIQUE (Optimisation) ===
// Rôle : Met à jour l'affichage d'une seule case sans toucher au reste
void afficherUneCase(Partie* partie, int gridX, int gridY, int curseurX, int curseurY, int selectX, int selectY) {
    // 1. Définition des constantes de position (DOIT ÊTRE IDENTIQUE À afficherNiveauJeu)
    int plateauX = 3;
    int plateauY = 5;

    // 2. Calcul de la position exacte sur l'écran console
    // Chaque case fait 4 caractères de large (" [x] ")
    int screenX = plateauX + 1 + (gridX * 4);
    int screenY = plateauY + 1 + gridY;

    // 3. Récupération de l'item
    int item = partie->tableau[gridY][gridX];

    // Symboles (reprise de ta liste statique)
    const char* SYMBOLES[] = {" ", "🍓", "🧅", "🍊", "🍇", "🥕", "💣", "🧱", "🦠", "🌵"};
    const int COULEURS[] = {7, 12, 13, 6, 13, 6, 15, 15, 10, 10}; // Codes couleurs simples

    // 4. Déplacement du curseur console
    allerA(screenX, screenY);

    // 5. Dessin logique (Curseur vs Sélection vs Normal)
    if (gridY == curseurY && gridX == curseurX) {
        changerCouleur(COULEUR_BLANC); printf("[");
        changerCouleur(COULEURS_ITEMS[item]); printf("%-2s", SYMBOLES[item]);
        changerCouleur(COULEUR_BLANC); printf("]");
    }
    else if (gridY == selectY && gridX == selectX) {
        changerCouleur(COULEUR_JAUNE); printf("{");
        changerCouleur(COULEURS_ITEMS[item]); printf("%-2s", SYMBOLES[item]);
        changerCouleur(COULEUR_JAUNE); printf("}");
    }
    else {
        changerCouleur(COULEURS_ITEMS[item]);
        printf(" %-2s ", SYMBOLES[item]);
    }
    changerCouleur(COULEUR_RESET); // Reset couleur
}

// === FONCTION DE SAISIE SÉCURISÉE ===
// Source : Assistant IA (Alternative sécurisée à scanf pour l'interface graphique)
// Rôle : Permet de taper son pseudo sans faire planter l'affichage graphique.
// Entrée : Le tableau où stocker le texte (buffer) et sa taille max.
// Sortie : Aucune (modifie directement le buffer).
void saisirPseudo(char* buffer, int longueurMax) {
    int i = 0; char c;
    memset(buffer, 0, longueurMax);
    while (1) {
        c = portableGetch();
        if (c == 13) break;
        else if (c == 8) { if (i > 0) { i--; printf("\b \b"); buffer[i] = '\0'; } }
        else if (i < longueurMax - 1 && c >= 32 && c <= 126) { buffer[i] = c; i++; printf("%c", c); }
    }
}
