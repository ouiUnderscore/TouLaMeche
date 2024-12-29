#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "entree_sortie.h"
#include "../arbre_de_prediction/arbre_pred.h"
#include "../sequence/sequence.h"

// Codes de couleur ANSI
#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[32m"
#define RED_COLOR "\033[31m"
#define YELLOW_COLOR "\033[33m"

void simule_entree_clavier(const char *input)
{
    FILE *f = fopen("input.txt", "w");
    if (!f)
    {
        perror("Erreur d'ouverture du fichier d'entrée");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%s", input);
    //printf("%s", input);
    fclose(f);
}

void test_readKeybord()
{
    printf(YELLOW_COLOR "[TEST] Test de la fonction readKeybord\n" RESET_COLOR);
    initArbre();
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);

    simule_entree_clavier("mot1 mot2 mot3\n");

    FILE *inputFile = fopen("input.txt", "r");
    if (!inputFile)
    {
        perror("Erreur d'ouverture du fichier d'entrée");
        exit(EXIT_FAILURE);
    }

    // Simule getchar
    FILE *stdinBackup = stdin;
    stdin = inputFile;
    readKeybord(&root, ht);
    fclose(inputFile);

    // Vérifie que l'arbre a bien été créé
    TreeNode *result1 = searchOrCreateTreeNode();
    assert(result1 != NULL);
    assert(strcmp(result1->mot, "mot3") == 0);

    stdin = stdinBackup;

    printf(GREEN_COLOR "Test readKeybord réussi.\n\n" RESET_COLOR);
}

void test_saveTreeNode()
{
    printf(YELLOW_COLOR "[TEST] Test de la fonction saveTreeNode\n" RESET_COLOR);
    initArbre();
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);
    simule_entree_clavier("mot1 mot2 mot3 mot1 mot2 mot3\n");

    // Fichier contenant l'entrée clavier
    FILE *inputFile = fopen("input.txt", "r");
    if (!inputFile)
    {
        perror("Erreur d'ouverture du fichier d'entrée");
        exit(EXIT_FAILURE);
    }

    // Simule getchar
    stdin = inputFile;
    readKeybord(&root, ht);
    fclose(inputFile);

    // Fichier qui contiendra le ngramme formatté
    FILE *fichierSortie = fopen("test_saveTreeNode.txt", "w");
    if (fichierSortie == NULL)
    {
        perror("Erreur d'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    // Sauvegarde arbre
    saveTreeNode(fichierSortie, &root, 0);
    fclose(fichierSortie);

    // Vérifie que l'arbre a bien été sauvegardé dans le fichier
    FILE *fichierEntree = fopen("test_saveTreeNode.txt", "r");
    if (fichierEntree == NULL)
    {
        perror("Erreur d'ouverture du fichier de lecture");
        exit(EXIT_FAILURE);
    }

    // Lire le contenu et vérifier qu'il correspond à l'arbre sauvegardé
    char ligne[BUFFER_SIZE];
    while (fgets(ligne, sizeof(ligne), fichierEntree))
    {
        ligne[strcspn(ligne, "\n")] = '\0';
        while (ligne[0] == '\t')
            memmove(ligne, ligne + 1, strlen(ligne));

        if (ligne[0] == '[' && strlen(ligne) > 2)
            assert(strstr(ligne, "[ p=") != NULL);
    }

    fclose(fichierEntree);
    printf(GREEN_COLOR "Test saveTreeNode réussi.\n\n" RESET_COLOR);
}

void test_loadTreeNode()
{
    printf(YELLOW_COLOR "[TEST] Test de la fonction loadTreeNode\n" RESET_COLOR);

    initArbre();

    // Charger l'arbre depuis le fichier généré précédemment
    FILE *fichierEntree = fopen("test_saveTreeNode.txt", "r");
    if (fichierEntree == NULL)
    {
        perror("Erreur d'ouverture du fichier de lecture");
        exit(EXIT_FAILURE);
    }

    loadTreeNode(fichierEntree, &root);
    fclose(fichierEntree);

    TreeNode *result1 = searchOrCreateTreeNode();
    assert(result1 != NULL);
    assert(strcmp(result1->mot, "mot2 ") == 0);
    assert(result1->nb_occurrences == 2);

    printf(GREEN_COLOR "Test loadTreeNode réussi.\n\n" RESET_COLOR);
}

int main()
{
    test_readKeybord();
    test_saveTreeNode();
    test_loadTreeNode();

    printf(GREEN_COLOR "Tous les tests sont passés avec succès.\n" RESET_COLOR);
    return 0;
}
