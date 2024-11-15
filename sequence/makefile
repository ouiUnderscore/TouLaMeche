# Nom de l'exécutable final
TARGET = test_sequence

# Fichiers sources
SRCS = test_sequence.c sequence.c

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
test_sequence.o: sequence.h
sequence.o: sequence.h hash.h
hash.h: list.h