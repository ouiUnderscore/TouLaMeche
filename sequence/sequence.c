// TODO: entete commentaire

#include "sequence.h"
#include <stdio.h>
#include <stdlib.h>

static int iterator;
static char *n_gramme[Lg_N_gramme];
static char *next_word;

// initialise le N-gramme avec des mots vides
void sequence_initialize(struct strhash_table *ht)
{
    ht = strhash_create(Lg_N_gramme);

    for (int i = 0; i < ht->size; i++)
        n_gramme[i] = NULL; // strhash_wordAdd(ht, NULL);
}

// Gestion de l'iterateur permettant de parcourir le N-gramme
// initialise l'iterateur sur le premier mot du N-gramme courant
void sequence_itStart(void)
{
    // L'itérateur c'est après le mot courant, c'est la position où on peut écrire
    iterator = 0;
}

// retourne le mot correspondant a la position de l'iterateur avance la position de l'iterateur.
const char *sequence_itNext(void)
{
    const char *current_word = n_gramme[iterator];
    iterator = (iterator + 1) % Lg_N_gramme;  // Avance circulairement
    return current_word;
}

// test si l'iterateur est a la fin du N-gramme
int sequence_itHasNext(void)
{
    // TODO:
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
    n_gramme[iterator] = next_word;

    // Met à jour l'index de l'itérateur pour "avancer" le buffer circulaire.
    // Si iterator atteint Lg_N_gramme, il revient à 0
    iterator = (iterator + 1) % Lg_N_gramme;
}

//
// Debug
//
// affiche le N-gramme courant, les mots sont séparés par des '/'
void sequence_print(void)
{
    int it_temp = iterator;

    for (int i = 0; i < Lg_N_gramme; i++)
    {
        if (it_temp == Lg_N_gramme)
        {
            printf("%s/", n_gramme[i]);
            it_temp = 0;
        }
        else
        {
            printf("%s/", n_gramme[i]);
            it_temp++;
        }
    }
    printf("\n");
}

// sequence sous forme d'une chaine por le test
// char *sequence_printInTab(void) {

// }