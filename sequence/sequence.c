// TODO: entete commentaire

#include "sequence.h"
#include <stdio.h>
#include <stdlib.h>

static int iterator; // Iterateur pour parcourir les éléments du n_gramme
static int position; // Position de la prochaine écriture
static char *n_gramme[Lg_N_gramme + 1];
static char *next_word;

// initialise le N-gramme avec des mots vides
void sequence_initialize(struct strhash_table *ht)
{
    for (int i = 0; i < (Lg_N_gramme + 1); i++)
        n_gramme[i] = NULL; // strhash_wordAdd(ht, NULL);

    position = 1;
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
    iterator = (iterator + 1) % (Lg_N_gramme + 1); // Avance circulairement
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

    // Réinitialise la case précédente avant de mettre à jour la position
    if (n_gramme[position] != NULL)
    {
        n_gramme[position] = NULL; // Réinitialise la case actuelle
    }
}

//
// Debug
//
// affiche le N-gramme courant, les mots sont séparés par des '/'
void sequence_print(void)
{
    int it_temp = iterator;

    for (int i = 0; i < (Lg_N_gramme + 1); i++)
    {
        if (it_temp == (Lg_N_gramme + 1))
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
    
    // Sans les nulls
    it_temp = iterator;
    for (int i = 0; i < (Lg_N_gramme + 1); i++)
    {
        if (n_gramme[i] != NULL)
        {
            if (it_temp == (Lg_N_gramme + 1))
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
    }
    printf("\n");
}

// sequence sous forme d'une chaine por le test
// char *sequence_printInTab(void) {

// }