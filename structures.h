#ifndef STRUCTURES_H
#define STRUCTURES_H

#define HAUTEUR 20
#define LARGEUR 15

#define NB_TYPES_ITEMS 5
#define NB_VIES_INITIAL 3
#define NB_NIVEAUX 3

// Structure d'une partie
typedef struct {
    int tableau[HAUTEUR][LARGEUR];
    int vies;
    int niveau;
    int coups_restants;
    int temps_restant;
    int contrat[NB_TYPES_ITEMS];
    int elimines[NB_TYPES_ITEMS];
} Partie;

// Structure pour sauvegarder une partie
typedef struct {
    char pseudo[50];
    int dernier_niveau;
    int vies_restantes;
} Sauvegarde;

#endif // STRUCTURES_H
