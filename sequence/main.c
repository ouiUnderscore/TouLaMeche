// TODO: entete commentaire

#include <stdio.h>
#include <stdlib.h>
#include "sequence.h"

int main()
{
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);

    // Test 1 : Ajout de mots dans le N-gramme
    printf("Ajout des mots dans le N-gramme :\n");
    sequence_addWord("aaa", ht);
    sequence_progress();
    sequence_addWord("bbb", ht);
    sequence_progress();
    sequence_addWord("ccc", ht);
    sequence_progress();

    // Test 2 : Affichage du N-gramme après l'ajout de 3 mots
    printf("\nN-gramme après ajout de 3 mots (attendu \"aaa/bbb/ccc/\") :\n");
    sequence_print();

    // Test 3 : Ajout d'un autre mot, et "avancement" du buffer circulaire
    printf("\nAjout du mot 'ddd' et avancement du buffer :\n");
    sequence_addWord("ddd", ht);
    sequence_progress();

    // Test 4 : Affichage du N-gramme après avoir ajouté le mot 'ddd'
    printf("\nN-gramme après ajout du mot 'ddd' (attendu \"ddd/bbb/ccc/\") :\n");
    sequence_print();

    // Test 5 : Test de l'itérateur
    sequence_itStart();

    printf("\nUtilisation de l'itérateur (attendu \"bbb ccc ddd \") :\n");
    while (sequence_itHasNext())
        printf("%s ", sequence_itNext());
    printf("\n");

    // Test 6 : Ajouter un autre mot pour voir l'évolution de l'itérateur
    printf("\nAjout d'un mot supplémentaire : 'eee'\n");
    sequence_addWord("eee", ht);
    sequence_progress();

    // Test 7 : Affichage du N-gramme après l'ajout du mot "eee"
    printf("\nN-gramme après ajout du mot 'eee' (attendu \"ddd/eee/ccc/\") :\n");
    sequence_print();

    // Test 8 : Tester l'itérateur après ajout d'un nouveau mot
    sequence_itStart();

    printf("\nUtilisation de l'itérateur après ajout du mot 'eee' (attendu \"ccc ddd eee \") :\n");
    while (sequence_itHasNext())
    {
        printf("%s ", sequence_itNext()); // Affiche chaque mot et son indice dans le parcours
    }
    printf("\n");

    return 0;
}
