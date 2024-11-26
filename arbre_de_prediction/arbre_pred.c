/******************************************************************************
 * Fichier : arbre_pred.c
 * Auteur : Emilie Zammit
 * Date de création : 24/11/2024
 * Description : Ce fichier implémente les fonctions définies dans arbre_pred.h.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre_pred.h"
#include "../tableau_dynamique/tab_dynamique.h"
#include "../sequence/sequence.h"
#include "../sequence/hash.h"

TreeNode t;

void initArbre()
{
    // Racine = mot vide
    t.mot = strdup("");
    t.nb_occurrences = 0;
    t.fils = createTabD(SIZE_TABD);
}

TreeNode *searchOrCreateTreeNode()
{
    sequence_itStart();
    TreeNode *currentTree = &t;

    while (sequence_itHasNext())
    {
        const char *currentWord = sequence_itNext();
        TreeNode *foundNode = NULL;

        for (int i = 0; i < currentTree->fils->size; i++)
        {
            TreeNode *enfant = readTabD(currentTree->fils, i);
            if (strcmp(enfant->mot, currentWord) == 0)
            {
                enfant->nb_occurrences++;
                foundNode = enfant;
                break;
            }
        }

        // Si non trouvé
        if (foundNode == NULL)
        {
            foundNode = (TreeNode *)malloc(sizeof(TreeNode));
            if (!foundNode)
            {
                perror("Erreur d'allocation mémoire pour un nœud de l'arbre");
                exit(EXIT_FAILURE);
            }
            foundNode->mot = strdup(currentWord);
            foundNode->nb_occurrences = 1;
            foundNode->fils = createTabD(SIZE_TABD);

            if (!addElemTabD(currentTree->fils, foundNode))
            {
                perror("Erreur d'ajout dans le tableau dynamique");
                free(foundNode->mot);
                free(foundNode->fils);
                free(foundNode);
                exit(EXIT_FAILURE);
            }
        }

        currentTree = foundNode;
    }

    return currentTree;
}

TreeNode *searchOrCreateLeaf(TreeNode *finalNode, const char *word)
{
    for (int i = 0; i < finalNode->fils->size; i++)
    {
        TreeNode *enfant = readTabD(finalNode->fils, i);
        if (strcmp(enfant->mot, word) == 0)
        {
            enfant->nb_occurrences++;
            return enfant;
        }
    }

    // Si non trouvé
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (!newNode)
    {
        perror("Erreur d'allocation mémoire pour un nœud de l'arbre");
        exit(EXIT_FAILURE);
    }
    newNode->mot = strdup(word);
    newNode->nb_occurrences = 1;
    newNode->fils = createTabD(SIZE_TABD);

    // Ajoute le nouveau nœud aux fils du nœud final
    if (!addElemTabD(finalNode->fils, newNode))
    {
        perror("Erreur d'ajout dans le tableau dynamique");
        free(newNode->mot);
        free(newNode->fils);
        free(newNode);
        return NULL;
    }

    return newNode;
}

TreeNode *maxOccurrence(TreeNode *finalNode)
{
    TreeNode *maxNode = NULL;
    int maxOccur = -1;

    for (int i = 0; i < finalNode->fils->size; i++)
    {
        TreeNode *enfant = readTabD(finalNode->fils, i);
        if (enfant->nb_occurrences > maxOccur)
            maxOccur = enfant->nb_occurrences;
        maxNode = enfant;
    }

    return maxNode;
}