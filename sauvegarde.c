#include <stdio.h>
#include <string.h>
#include "sauvegarde.h"

// === SAUVEGARDE ===
// Source : Assistant IA (Algorithme de mise à jour d'un fichier texte sans doublons)
// Rôle : Met à jour la sauvegarde d'un joueur ou en crée une nouvelle.
// Entrée : Pseudo, Niveau atteint, Vies restantes.
// Sortie : 1 si succès, 0 si erreur fichier.
int sauvegarderPartie(char* pseudo, int niveau, int vies) {
    Enregistrement saves[100]; // Limite : 100 joueurs max
    int count = 0;
    int joueurTrouve = 0;

    // 1. LECTURE : On charge tout le fichier en mémoire RAM
    FILE* fichierLecture = fopen(FICHIER_SAUVEGARDE, "r");
    if (fichierLecture != NULL) {
        while (count < 100 && fscanf(fichierLecture, "%s %d %d",
               saves[count].pseudo,
               &saves[count].niveau,
               &saves[count].vies) == 3) {

            // Si le joueur existe déjà, on met à jour ses infos
            if (strcmp(saves[count].pseudo, pseudo) == 0) {
                saves[count].niveau = niveau;
                saves[count].vies = vies;
                joueurTrouve = 1;
            }
            count++;
        }
        fclose(fichierLecture);
    }

    // 2. AJOUT : Si nouveau joueur, on l'ajoute à la liste
    if (!joueurTrouve && count < 100) {
        strcpy(saves[count].pseudo, pseudo);
        saves[count].niveau = niveau;
        saves[count].vies = vies;
        count++;
    }

    // 3. ÉCRITURE : On réécrit tout le fichier proprement
    FILE* fichierEcriture = fopen(FICHIER_SAUVEGARDE, "w");
    if (fichierEcriture == NULL) return 0;

    for (int i = 0; i < count; i++) {
        fprintf(fichierEcriture, "%s %d %d\n", saves[i].pseudo, saves[i].niveau, saves[i].vies);
    }
    fclose(fichierEcriture);

    return 1;
}

// === CHARGEMENT ===
// Rôle : Cherche un pseudo et récupère sa progression.
// Entrée : Pseudo cherché.
// Sortie (via pointeurs) : Le niveau et les vies du joueur.
int chargerPartie(char* pseudo, int* niveau, int* vies) {
    FILE* fichier = fopen(FICHIER_SAUVEGARDE, "r");
    if (fichier == NULL) return 0;

    char pseudoLu[50];
    int niveauLu, viesLu;

    // On parcourt le fichier ligne par ligne
    while (fscanf(fichier, "%s %d %d", pseudoLu, &niveauLu, &viesLu) == 3) {
        if (strcmp(pseudoLu, pseudo) == 0) {
            *niveau = niveauLu;
            *vies = viesLu;
            fclose(fichier);
            return 1; // Joueur trouvé
        }
    }

    fclose(fichier);
    return 0; // Joueur pas trouvé
}
int sauvegarderPartieComplete(char* pseudo, Partie* partie) {
    char nomFichier[100];
    sprintf(nomFichier, "%s.sav", pseudo); // Ex: "Rems.sav"

    FILE* f = fopen(nomFichier, "wb"); // wb = Write Binary
    if (f == NULL) return 0;

    // On écrit directement toute la structure d'un coup
    fwrite(partie, sizeof(Partie), 1, f);

    fclose(f);
    return 1;
}

// Rôle : Charge l'état exact du jeu depuis le fichier binaire
int chargerPartieComplete(char* pseudo, Partie* partie) {
    char nomFichier[100];
    sprintf(nomFichier, "%s.sav", pseudo);

    FILE* f = fopen(nomFichier, "rb"); // rb = Read Binary
    if (f == NULL) return 0; // Pas de sauvegarde complète trouvée

    // On lit les données directement dans la structure
    fread(partie, sizeof(Partie), 1, f);

    fclose(f);
    return 1;
}
