/******************************************************************************
 * Fichier : test_arbre_pred.c
 * Auteur : Emilie Zammit
 * Date de création : 24/11/2024
 * Description : Ce fichier teste les fonctions définies dans arbre_pred.h.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "arbre_pred.h"
#include "../sequence/sequence.h"
#include "../sequence/hash.h"

// Codes de couleur ANSI
#define COLOR_WHITE "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_ORANGE "\033[33m"

void afficherArbre(TreeNode *node, int niveau)
{
    if (node == NULL)
        return;

    for (int i = 0; i < niveau; i++)
        printf("  ");
    printf("- %s (occurrences : %d)\n", node->mot, node->nb_occurrences);

    for (int i = 0; i < node->fils->size; i++)
    {
        TreeNode *enfant = (TreeNode *)readTabD(node->fils, i);
        afficherArbre(enfant, niveau + 1);
    }
}

void test_initArbre()
{
    printf(COLOR_ORANGE "[TEST] Initialisation de l'arbre\n" COLOR_WHITE);
    initArbre();
    assert(strcmp(t.mot, "") == 0);
    assert(t.nb_occurrences == 0);
    assert(t.fils != NULL);
    printf(COLOR_GREEN "Test d'initialisation réussi : Le TreeNode a été initialisé correctement.\n" COLOR_WHITE);
}

void test_searchOrCreateTreeNode()
{
    printf(COLOR_ORANGE "\n[TEST] Recherche ou création de branches dans l'arbre\n" COLOR_WHITE);
    initArbre();
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);

    // mot1 -> mot2 -> mot3
    printf(COLOR_WHITE "- Ajout de la séquence mot1 -> mot2 -> mot3\n" COLOR_WHITE);
    sequence_addWord("mot1", ht);
    sequence_progress();
    sequence_addWord("mot2", ht);
    sequence_progress();
    sequence_addWord("mot3", ht);
    sequence_progress();

    TreeNode *result1 = searchOrCreateTreeNode();
    assert(result1 != NULL);
    assert(strcmp(result1->mot, "mot3") == 0);
    printf(COLOR_GREEN "Séquence mot1 -> mot2 -> mot3 ajoutée avec succès.\n" COLOR_WHITE);

    // Vérification des enfants
    TreeNode *child1 = readTabD(t.fils, 0);
    assert(child1 != NULL && strcmp(child1->mot, "mot1") == 0);
    TreeNode *child2 = readTabD(child1->fils, 0);
    assert(child2 != NULL && strcmp(child2->mot, "mot2") == 0);
    TreeNode *child3 = readTabD(child2->fils, 0);
    assert(child3 != NULL && strcmp(child3->mot, "mot3") == 0);

    // mot4 -> mot5 -> mot6
    printf(COLOR_WHITE "- Ajout de la séquence mot4 -> mot5 -> mot6\n" COLOR_WHITE);
    sequence_addWord("mot4", ht);
    sequence_progress();
    sequence_addWord("mot5", ht);
    sequence_progress();
    sequence_addWord("mot6", ht);
    sequence_progress();
    TreeNode *result4 = searchOrCreateTreeNode();
    assert(result4 != NULL);
    assert(strcmp(result4->mot, "mot6") == 0);
    printf(COLOR_GREEN "Séquence mot4 -> mot5 -> mot6 ajoutée avec succès.\n" COLOR_WHITE);

    // Affichage de l'arbre
    printf(COLOR_WHITE "Arbre après ajout des séquences :\n" COLOR_WHITE);
    afficherArbre(&t, 0);

    strhash_free(ht);
    printf(COLOR_GREEN "Test de recherche et création avec plusieurs branches réussi.\n" COLOR_WHITE);
}

void test_searchOrCreateLeaf()
{
    printf(COLOR_ORANGE "\n[TEST] Recherche ou création de feuilles dans l'arbre\n" COLOR_WHITE);
    initArbre();
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);

    sequence_addWord("mot1", ht);
    sequence_progress();
    sequence_addWord("mot2", ht);
    sequence_progress();
    sequence_addWord("mot3", ht);
    sequence_progress();

    TreeNode *finalNode = searchOrCreateTreeNode();
    assert(finalNode != NULL);
    assert(strcmp(finalNode->mot, "mot3") == 0);

    // Nouvelle feuille
    const char *newWord = "mot_suivant";
    TreeNode *result1 = searchOrCreateLeaf(finalNode, newWord);
    assert(result1 != NULL);
    assert(strcmp(result1->mot, newWord) == 0);
    assert(result1->nb_occurrences == 1);
    printf(COLOR_GREEN "Ajout du mot '%s' réussi (occurrences : %d).\n" COLOR_WHITE, newWord, result1->nb_occurrences);

    // Feuille existante
    TreeNode *result2 = searchOrCreateLeaf(finalNode, newWord);
    assert(result2 != NULL);
    assert(strcmp(result2->mot, newWord) == 0);
    assert(result2->nb_occurrences == 2);
    printf(COLOR_GREEN "Incrémentation du mot '%s' réussie (occurrences : %d).\n" COLOR_WHITE, newWord, result2->nb_occurrences);

    // Nouvelle feuille
    const char *anotherWord = "autre_mot";
    TreeNode *result3 = searchOrCreateLeaf(finalNode, anotherWord);
    assert(result3 != NULL);
    assert(strcmp(result3->mot, anotherWord) == 0);
    assert(result3->nb_occurrences == 1);
    printf(COLOR_GREEN "Ajout d'un autre mot '%s' réussi.\n" COLOR_WHITE, anotherWord);

    printf(COLOR_WHITE "Arbre après les ajouts :\n" COLOR_WHITE);
    afficherArbre(&t, 0);
    strhash_free(ht);
    printf(COLOR_GREEN "Test de searchOrCreateLeaf terminé avec succès.\n" COLOR_WHITE);
}

void test_maxOccurrence()
{
    printf(COLOR_ORANGE "\n[TEST] Recherche du mot avec le maximum d'occurrences\n" COLOR_WHITE);
    initArbre();
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);

    // mot1 -> mot2 -> mot3
    sequence_addWord("mot1", ht);
    sequence_progress();
    sequence_addWord("mot2", ht);
    sequence_progress();
    sequence_addWord("mot3", ht);
    sequence_progress();
    TreeNode *finalNode = searchOrCreateTreeNode();

    // feuilles
    searchOrCreateLeaf(finalNode, "mot_suivant1");
    searchOrCreateLeaf(finalNode, "mot_suivant2");
    searchOrCreateLeaf(finalNode, "mot_suivant2");
    searchOrCreateLeaf(finalNode, "mot_suivant3");
    searchOrCreateLeaf(finalNode, "mot_suivant3");
    searchOrCreateLeaf(finalNode, "mot_suivant3");

    // rechercheMaxOccur
    TreeNode *maxNode = maxOccurrence(finalNode);
    assert(maxNode != NULL);
    assert(strcmp(maxNode->mot, "mot_suivant3") == 0);
    assert(maxNode->nb_occurrences == 3);

    afficherArbre(&t, 0);
    printf(COLOR_GREEN "Test réussi : Le mot avec le maximum d'occurrences est '%s' (%d occurrences).\n" COLOR_WHITE, maxNode->mot, maxNode->nb_occurrences);
    strhash_free(ht);
}

int main()
{
    test_initArbre();
    test_searchOrCreateTreeNode();
    test_searchOrCreateLeaf();
    test_maxOccurrence();
    return 0;
}
