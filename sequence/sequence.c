// TODO: entete commentaire

#include "sequence.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEBUG
#define BUFFER_LENGTH 1024
static char debug_buffer[BUFFER_LENGTH + 1];

static int iterator; // Iterateur pour parcourir les éléments du n_gramme
static int position; // Position de la prochaine écriture
static char *n_gramme[Lg_N_gramme + 1];
static char *next_word;

// initialise le N-gramme avec des mots vides
void sequence_initialize(struct strhash_table *ht)
{
    for (int i = 0; i < Lg_N_gramme + 1; i++)
        n_gramme[i] = "";

    position = 0;
}

// Gestion de l'iterateur permettant de parcourir le N-gramme
// initialise l'iterateur sur le premier mot du N-gramme courant
void sequence_itStart(void)
{
    // L'itérateur c'est après le mot courant, c'est la position où on peut écrire
    iterator = (position + 1) % (Lg_N_gramme + 1);
}

// retourne le mot correspondant a la position de l'iterateur avance la position de l'iterateur.
const char *sequence_itNext(void)
{
    const char *current_word = n_gramme[iterator];
    iterator = (iterator + 1) % (Lg_N_gramme + 1);
    return current_word;
}

// test si l'iterateur est a la fin du N-gramme
int sequence_itHasNext(void)
{
    return iterator != position;
}

// Nouveau mot de fin du prochain N-gramme definit le nouveau mot qui entrera dans le N-gramme
void sequence_addWord(const char *word, struct strhash_table *ht)
{
    next_word = strhash_wordAdd(ht, word);
}

// retourne le nouveau mot qui entrera dans le N-gramme
const char *sequence_nextWord(void)
{
    return next_word;
}

// avance le N gramme pour integrer le nouveau mot ecrit
void sequence_progress(void)
{
    // Ajoute le nouveau mot à la position courante de l'itérateur
    n_gramme[position] = next_word;

    // Met à jour l'index de l'itérateur pour "avancer" le buffer circulaire.
    // Si iterator atteint Lg_N_gramme, il revient à 0
    position = (position + 1) % (Lg_N_gramme + 1);

    // Transforme la nouvelle position en "" pour indiquer qu'elle est prête pour un nouveau mot
    n_gramme[position] = "";
}

//
// Debug
//
// affiche le N-gramme courant, les mots sont séparés par des '/'
void sequence_print(void)
{
    for (int i = 0; i < Lg_N_gramme + 1; i++)
        if (i != position)
            printf("%s/", n_gramme[i]);
        else
            printf("(pos)/");
    printf("\n");
}

// sequence sous forme d'une chaine por le test
char *sequence_printInTab(void)
{
    debug_buffer[0] = '\0';
    int length = 0;

    for (int i = 0; i < Lg_N_gramme + 1; i++) {
        const char *word = n_gramme[i];

        if (strcmp(word, "") == 0)
            word = "(null)";

        length += strlen(word) + 1;

        if (length > BUFFER_LENGTH)
        {
            fprintf(stderr, "Erreur : La taille du buffer est trop petite\n");
            return NULL;
        }

        strcat(debug_buffer, word);
        strcat(debug_buffer, "/");
    }

    return debug_buffer;
}