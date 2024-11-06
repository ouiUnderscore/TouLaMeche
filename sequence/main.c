// TODO: entete commentaire

#include <stdio.h>
#include <stdlib.h>
#include "sequence.h"

int main()
{
    // Crée un tableau pour tester le N-gramme
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    
    // Initialise le N-gramme
    sequence_initialize(ht);

    // Test 1 : Ajout de mots dans le N-gramme
    sequence_addWord("Hello", ht);
    sequence_progress();  // Avance le buffer circulaire
    sequence_addWord("world", ht);
    sequence_progress();  // Avance le buffer circulaire
    sequence_addWord("this", ht);
    sequence_progress();  // Avance le buffer circulaire

    // Test 2 : Affichage du N-gramme courant
    printf("N-gramme après ajout de 3 mots :\n");
    sequence_print();  // Affiche "Hello/world/this/"

    // Test 3 : Ajout d'un autre mot, et "avancement" du buffer circulaire
    sequence_addWord("is", ht);
    sequence_progress();  // Avance le buffer circulaire

    // Test 4 : Affichage du N-gramme après avoir ajouté un mot supplémentaire
    printf("\nN-gramme après ajout du mot 'is' :\n");
    sequence_print();  // Affiche "world/this/is/"

    // Test 5 : Test de l'itérateur
    sequence_itStart();  // Démarre l'itérateur

    printf("\nUtilisation de l'itérateur :\n");
    while (sequence_itHasNext()) {
        printf("%s ", sequence_itNext());
    }
    // Cela devrait afficher "world this is "
    printf("\n");

    return 0;
}
