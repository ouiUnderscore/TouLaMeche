# Nom de l'exécutable final
TARGET = test_tab_dynamique

# Fichiers sources
SRCS = test_tab_dynamique.c tab_dynamique.c

# Fichiers d'objets
OBJS = $(SRCS:.c=.o) 

# Options de compilation
CC = gcc
CFLAGS = -Wall

# Règle par défaut : compiler l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Compiler chaque fichier .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dépendances
test_tab_dynamique.o: tab_dynamique.h
tab_dynamique.o: tab_dynamique.h