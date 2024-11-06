/* ---------------------------------------------------------------------------
 * Gestion des N-grammes
 * ---------------------------------------------------------------------------
 *  01/10/2024	SR	Creation
 *  12/10/2024	SR	Ajout de la fct printInTTab
 * ---------------------------------------------------------------------------
 */

#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_
#include "hash.h"

// nombre de mots dans les N-grammes
enum
{
	Lg_N_gramme = 3
};

// ---------------------------------------------------------
// initialise le N-gramme avec des mots vides
void sequence_initialize(struct strhash_table *ht);

// ---------------------------------------------------------
// Gestion de l'iterateur permettant de parcourir le N-gramme
// initialise l'iterateur sur le premier mot du N-gramme courant
void sequence_itStart(void);

// retourne le mot correspondant a la position de l iterateur
// avance la position de l'eterateur.
const char *sequence_itNext(void);

// test si l'iterateur est a la fin du N-gramme
int sequence_itHasNext(void);

// ---------------------------------------------------------
// Nouveau mot de fin du prochain N-gramme
// definit le nouveau mot qui entrera dans le N-gramme
void sequence_addWord(const char *word, struct strhash_table *ht);

// retourne le nouveau mot qui entrera dans le N-gramme
const char *sequence_nextWord(void);

// avance le N gramme pour integrer le nouveaua mot ecrit
void sequence_progress(void);

// ---------------------------------------------------------
// Debug
// affiche le N-gramme courant, les mots sont séparés par des '/'
void sequence_print(void);
// sequence sous forme d une chaine por le test
char *sequence_printInTab(void);

#endif
