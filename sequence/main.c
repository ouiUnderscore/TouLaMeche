// TODO: entete commentaire

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "sequence.h"
#include "string.h"

#define BUFFER_LENGTH 1024

static void test_initialization(struct strhash_table *ht)
{
    assert(ht != NULL);
    if (ht == NULL)
    {
        fprintf(stderr, "Erreur : La table de hachage n'a pas pu être créée.\n");
        assert(0);
    }

    sequence_initialize(ht);
    printf("Test d'initialisation réussi : La table de hachage et la séquence ont été correctement initialisées.\n");
}

static void assertEquals(const char *test, const char *attendu)
{
    if (test == NULL && attendu != NULL)
    {
        fprintf(stderr, "Erreur : La valeur test est NULL, mais la valeur attendue est '%s'.\n", attendu);
        assert(0);
    }
    else if (test != NULL && attendu == NULL)
    {
        fprintf(stderr, "Erreur : La valeur test est '%s', mais la valeur attendue est NULL.\n", test);
        assert(0);
    }
    else if (strcmp(test, attendu) != 0)
    {
        fprintf(stderr, "Erreur : La valeur test ('%s') ne correspond pas à la valeur attendue ('%s').\n", test, attendu);
        assert(0);
    }
}

static void test_addWord(const char *mot, struct strhash_table *ht, const char *attendu)
{
    sequence_addWord(mot, ht);
    sequence_progress();

    char *result = sequence_printInTab();

    assertEquals(result, attendu);
    printf("Test d'ajout de mot réussi : Le mot '%s' a été correctement ajouté. Résultat : %s\n", mot, result);
}

static void test_iterator(struct strhash_table *ht, const char *expected_output)
{
    sequence_itStart();
    char result[BUFFER_LENGTH] = "";
    
    while (sequence_itHasNext()) {
        const char *word = sequence_itNext();
        strcat(result, word);
        strcat(result, " ");
    }

    assertEquals(result, expected_output);
    printf("Test de l'itérateur réussi : La séquence a été parcourue correctement. Résultat : %s\n", result);
}

int main()
{
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    test_initialization(ht);

    test_addWord("mot1", ht, "mot1/(null)/(null)/(null)/");
    test_addWord("mot2", ht, "mot1/mot2/(null)/(null)/");
    test_addWord("mot3", ht, "mot1/mot2/mot3/(null)/");
    test_addWord("mot4", ht, "(null)/mot2/mot3/mot4/");
    test_addWord("mot5", ht, "mot5/(null)/mot3/mot4/");

    test_iterator(ht, "mot3 mot4 mot5 ");
    test_iterator(ht, "mot3 mot4 mot5 ");

    test_addWord("mot6", ht, "mot5/mot6/(null)/mot4/");
    test_addWord("mot7", ht, "mot5/mot6/mot7/(null)/");
    test_addWord("mot8", ht, "(null)/mot6/mot7/mot8/");

    test_iterator(ht, "mot6 mot7 mot8 ");

    return 0;
}
