# Nom de l'exécutable final
TARGET = main

# Fichiers sources
SRCS = main.c sequence.c

# Fichiers d'objets
OBJS = $(SRCS:.c=.o) hash_x86_64.o list_x86_64.o

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
main.o: sequence.h
sequence.o: sequence.h hash.h
hash.h: list.h

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)
