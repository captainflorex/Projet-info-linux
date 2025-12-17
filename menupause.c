#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#include "menupause.h"
#include "affichage.h"   // Pour effacerEcran, couleurs, saisirPseudo...
#include "sauvegarde.h"  // Pour sauvegarderPartieComplete

// Fonction interne pour dessiner le menu (privée à ce fichier)
static void afficherDesignMenuPause(void) {
    effacerEcran();
    changerCouleur(COULEUR_JAUNE);
    printf("\n\n");
    printf("    ╔════════════════════════════════════════════╗\n");
    printf("    ║                JEU EN PAUSE                ║\n");
    printf("    ╚════════════════════════════════════════════╝\n\n");

    changerCouleur(COULEUR_BLANC);
    printf("      1. "); changerCouleur(COULEUR_VERT); printf("Reprendre la partie\n\n");
    
    changerCouleur(COULEUR_BLANC);
    printf("      2. "); changerCouleur(COULEUR_CYAN); printf("Sauvegarder et Quitter\n\n");
    
    changerCouleur(COULEUR_BLANC);
    printf("      3. "); changerCouleur(COULEUR_ROUGE); printf("Abandonner (Menu Principal)\n\n");

    changerCouleur(COULEUR_JAUNE);
    printf("    >> Votre choix : ");
    changerCouleur(COULEUR_RESET);
}

// Fonction Principale du module
// Source : Aide IA (Logique de déport du temps et gestion des inputs)
int gererMenuPause(Partie* partie, clock_t* debutNiveau) {
    // 1. On note l'heure où la pause commence
    clock_t debutPause = clock();
    int resterDansMenu = 1;
    int codeRetour = 0; // 0 = Reprendre, 1 = Quitter

    while (resterDansMenu) {
        afficherDesignMenuPause();
        char choix = getch();

        switch (choix) {
            case '1': // REPRENDRE
                resterDansMenu = 0;
                codeRetour = 0; 
                break;

            case '2': // SAUVEGARDER & QUITTER
            {
                char pseudo[50];
                effacerEcran();
                changerCouleur(COULEUR_CYAN);
                printf("\n  >> Nom de la sauvegarde : ");
                changerCouleur(COULEUR_BLANC);
                
                // On utilise la fonction déplacée dans affichage.c
                saisirPseudo(pseudo, 50);

                if (sauvegarderPartieComplete(pseudo, partie)) {
                    changerCouleur(COULEUR_VERT);
                    printf("\n\n  >> Sauvegarde reussie !");
                } else {
                    changerCouleur(COULEUR_ROUGE);
                    printf("\n\n  >> Erreur de sauvegarde.");
                }
                Sleep(1500);
                resterDansMenu = 0;
                codeRetour = 1; // On signale qu'on veut quitter
            }
            break;

            case '3': // ABANDONNER
                resterDansMenu = 0;
                codeRetour = 1; // On signale qu'on veut quitter
                break;
        }
    }

    // 2. Si on reprend le jeu, on calcule la durée de la pause
    if (codeRetour == 0) {
        clock_t finPause = clock();
        // On décale le chronomètre initial du jeu de la durée de la pause
        // Comme ça, le joueur n'a pas perdu de temps de jeu
        *debutNiveau += (finPause - debutPause);
    }

    return codeRetour;
}
