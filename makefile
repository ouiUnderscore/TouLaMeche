# Nom des exécutables finaux
TARGET_ARBRE = test_arbre_pred
TARGET_SEQ = test_sequence
TARGET_TAB = test_tab_dynamique
TARGET_ENTREE_SORTIE = test_entree_sortie

# Fichiers sources
SRCS_ARBRE = arbre_de_prediction/test_arbre_pred.c arbre_de_prediction/arbre_pred.c sequence/sequence.c sequence/hash.c sequence/list.c
SRCS_SEQ = sequence/test_sequence.c sequence/sequence.c sequence/hash.c sequence/list.c
SRCS_TAB = tableau_dynamique/test_tab_dynamique.c tableau_dynamique/tab_dynamique.c
SRCS_ENTREE_SORTIE = entree_sortie/test_entree_sortie.c entree_sortie/entree_sortie.c sequence/sequence.c sequence/hash.c sequence/list.c arbre_de_prediction/arbre_pred.c

# Fichiers objets
OBJS_ARBRE = arbre_de_prediction/test_arbre_pred.o arbre_de_prediction/arbre_pred.o tableau_dynamique/tab_dynamique.o sequence/sequence.o sequence/hash_x86_64.o sequence/list_x86_64.o
OBJS_SEQ = sequence/test_sequence.o sequence/sequence.o sequence/hash_x86_64.o sequence/list_x86_64.o
OBJS_TAB = tableau_dynamique/test_tab_dynamique.o tableau_dynamique/tab_dynamique.o
OBJS_ENTREE_SORTIE = entree_sortie/test_entree_sortie.o entree_sortie/entree_sortie.o sequence/sequence.o sequence/hash_x86_64.o sequence/list_x86_64.o arbre_de_prediction/arbre_pred.o tableau_dynamique/tab_dynamique.o

# Options de compilation
CC = gcc
CFLAGS = -Wall

# Règle par défaut : compiler tous les exécutables
all: $(TARGET_ARBRE) $(TARGET_SEQ) $(TARGET_TAB) $(TARGET_ENTREE_SORTIE)

# Compiler l'exécutable de l'arbre de prédiction
$(TARGET_ARBRE): $(OBJS_ARBRE)
	$(CC) $(CFLAGS) -o $(TARGET_ARBRE) $(OBJS_ARBRE) -lm

# Compiler l'exécutable de la séquence
$(TARGET_SEQ): $(OBJS_SEQ)
	$(CC) $(CFLAGS) -o $(TARGET_SEQ) $(OBJS_SEQ) -lm

# Compiler l'exécutable du tableau dynamique
$(TARGET_TAB): $(OBJS_TAB)
	$(CC) $(CFLAGS) -o $(TARGET_TAB) $(OBJS_TAB) -lm

# Compiler l'exécutable des entrées/sorties
$(TARGET_ENTREE_SORTIE): $(OBJS_ENTREE_SORTIE)
	$(CC) $(CFLAGS) -o $(TARGET_ENTREE_SORTIE) $(OBJS_ENTREE_SORTIE) -lm

# Compiler chaque fichier .c en .o pour l'arbre de prédiction
arbre_de_prediction/%.o: arbre_de_prediction/%.c arbre_de_prediction/arbre_pred.h sequence/sequence.h sequence/hash.h sequence/list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compiler chaque fichier .c en .o pour la séquence
sequence/%.o: sequence/%.c sequence/sequence.h sequence/hash.h sequence/list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compiler chaque fichier .c en .o pour le tableau dynamique
tableau_dynamique/%.o: tableau_dynamique/%.c tableau_dynamique/tab_dynamique.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compiler chaque fichier .c en .o pour les entrées/sorties
entree_sortie/%.o: entree_sortie/%.c entree_sortie/entree_sortie.h sequence/sequence.h sequence/hash.h sequence/list.h arbre_de_prediction/arbre_pred.h
	$(CC) $(CFLAGS) -c $< -o $@

# Dépendances pour l'arbre de prédiction
arbre_de_prediction/test_arbre_pred.o: arbre_de_prediction/arbre_pred.h
arbre_de_prediction/arbre_pred.o: arbre_de_prediction/arbre_pred.h

# Dépendances pour la séquence
sequence/test_sequence.o: sequence/sequence.h
sequence/sequence.o: sequence/sequence.h sequence/hash.h
sequence/hash_x86_64.o: sequence/hash.h
sequence/list_x86_64.o: sequence/list.h

# Dépendances pour le tableau dynamique
tableau_dynamique/test_tab_dynamique.o: tableau_dynamique/tab_dynamique.h
tableau_dynamique/tab_dynamique.o: tableau_dynamique/tab_dynamique.h

# Dépendances pour les entrées/sorties
entree_sortie/test_entree_sortie.o: entree_sortie/entree_sortie.h sequence/sequence.h
entree_sortie/entree_sortie.o: entree_sortie/entree_sortie.h sequence/sequence.h

# Nettoyage des fichiers objets et des exécutables, sans supprimer hash_x86_64.o et list_x86_64.o
clean:
	rm -f $(filter-out sequence/hash_x86_64.o sequence/list_x86_64.o, $(OBJS_ARBRE) $(OBJS_SEQ) $(OBJS_TAB) $(OBJS_ENTREE_SORTIE)) $(TARGET_ARBRE) $(TARGET_SEQ) $(TARGET_TAB) $(TARGET_ENTREE_SORTIE)
