# Nom de l'exécutable final pour l'arbre de prédiction
TARGET_ARBRE = test_arbre_pred

# Nom de l'exécutable final pour la séquence
TARGET_SEQ = test_sequence

# Nom de l'exécutable final pour le tableau dynamique
TARGET_TAB = test_tab_dynamique

# Fichiers sources pour l'arbre de prédiction
SRCS_ARBRE = arbre_de_prediction/test_arbre_pred.c arbre_de_prediction/arbre_pred.c sequence/sequence.c sequence/hash.c sequence/list.c

# Fichiers sources pour la séquence
SRCS_SEQ = sequence/test_sequence.c sequence/sequence.c sequence/hash.c sequence/list.c

# Fichiers sources pour le tableau dynamique
SRCS_TAB = tableau_dynamique/test_tab_dynamique.c tableau_dynamique/tab_dynamique.c

# Fichiers objets pour l'arbre de prédiction
OBJS_ARBRE = arbre_de_prediction/test_arbre_pred.o arbre_de_prediction/arbre_pred.o tableau_dynamique/tab_dynamique.o sequence/sequence.o sequence/hash_x86_64.o sequence/list_x86_64.o

# Fichiers objets pour la séquence
OBJS_SEQ = sequence/test_sequence.o sequence/sequence.o sequence/hash_x86_64.o sequence/list_x86_64.o

# Fichiers objets pour le tableau dynamique
OBJS_TAB = tableau_dynamique/test_tab_dynamique.o tableau_dynamique/tab_dynamique.o

# Options de compilation
CC = gcc
CFLAGS = -Wall

# Règle par défaut : compiler l'exécutable final pour chaque module
all: $(TARGET_ARBRE) $(TARGET_SEQ) $(TARGET_TAB)

# Compiler l'exécutable de l'arbre de prédiction
$(TARGET_ARBRE): $(OBJS_ARBRE)
	$(CC) $(CFLAGS) -o $(TARGET_ARBRE) $(OBJS_ARBRE) -lm

# Compiler l'exécutable de la séquence
$(TARGET_SEQ): $(OBJS_SEQ)
	$(CC) $(CFLAGS) -o $(TARGET_SEQ) $(OBJS_SEQ) -lm

# Compiler l'exécutable du tableau dynamique
$(TARGET_TAB): $(OBJS_TAB)
	$(CC) $(CFLAGS) -o $(TARGET_TAB) $(OBJS_TAB) -lm

# Compiler chaque fichier .c en .o pour l'arbre de prédiction
arbre_de_prediction/%.o: arbre_de_prediction/%.c arbre_de_prediction/arbre_pred.h sequence/sequence.h sequence/hash.h sequence/list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compiler chaque fichier .c en .o pour la séquence
sequence/%.o: sequence/%.c sequence/sequence.h sequence/hash.h sequence/list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compiler chaque fichier .c en .o pour le tableau dynamique
tableau_dynamique/%.o: tableau_dynamique/%.c tableau_dynamique/tab_dynamique.h
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

# Nettoyage des fichiers objets et des exécutables
clean:
	rm -f $(OBJS_ARBRE) $(OBJS_SEQ) $(OBJS_TAB) $(TARGET_ARBRE) $(TARGET_SEQ) $(TARGET_TAB)
