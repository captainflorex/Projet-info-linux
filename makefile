# Nom de l'exécutable
EXEC = ece_heroes

# Compilateur
CC = gcc

# AJOUT : Options pour générer les fichiers RTL nécessaires à egypt
# On les sépare pour pouvoir les activer ou non
GRAPH_FLAGS = -fdump-rtl-expand

# Options de compilation originales
CFLAGS = -Wall -O3 -I.

# Liste des fichiers sources
SRC = main.c affichage.c moteur.c sauvegarde.c menupause.c
OBJ = $(SRC:.c=.o)

# Détection de l'OS
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXT = .exe
else
    RM = rm -f
    EXT =
endif

# Règle principale
$(EXEC)$(EXT): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)$(EXT)
	@echo "Compilation terminée avec succès !"

# MODIFICATION : On ajoute $(GRAPH_FLAGS) ici
%.o: %.c
	$(CC) $(CFLAGS) $(GRAPH_FLAGS) -c $< -o $@

# AJOUT : Règle pour générer le graphe d'appel
graph:
	# On vérifie d'abord si des fichiers expand existent
	@ls *.expand > /dev/null 2>&1 || (echo "Erreur: Aucun fichier .expand trouvé. Tapez 'make' d'abord."; exit 1)
	# Utilisation de *.expand (ou *.*.expand pour être sûr)
	egypt $$(ls *.expand) | dot -Tpng -o callgraph.png -Grankdir=LR
	@echo "Graphe généré dans callgraph.png"
# Nettoyage (mis à jour pour supprimer les fichiers .expand)
clean:
	$(RM) $(OBJ) $(EXEC)$(EXT) *.expand callgraph.png
	@echo "Nettoyage effectué."

.PHONY: clean graph