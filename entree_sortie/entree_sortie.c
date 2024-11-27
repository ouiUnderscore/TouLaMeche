#include <stdio.h>
#include <string.h>
#include "entree_sortie.h"

// Codes de couleur ANSI
#define COLOR_WHITE "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_ORANGE "\033[33m"

void afficherArbre2(TreeNode *node, int niveau)
{
    if (node == NULL)
        return;

    for (int i = 0; i < niveau; i++)
        printf("  ");
    printf("- %s (occurrences : %d)\n", node->mot, node->nb_occurrences);

    for (int i = 0; i < node->fils->size; i++)
    {
        TreeNode *enfant = (TreeNode *)readTabD(node->fils, i);
        afficherArbre2(enfant, niveau + 1);
    }
}

void debugArbre(TreeNode *racine)
{
    printf("Affichage complet de l'arbre :\n");
    afficherArbre2(racine, 0);
}

void readKeybord(FILE *fichierSortie, TreeNode *racine, struct strhash_table *ht)
{
    char c;
    char buffer[256];
    int i = 0;
    int predictionMode = 0;

    while ((c = getchar()) != CHAR_FIN)
    {
        int est_separateur = 0;
        for (int j = 0; j < strlen(SEPARATEURS); j++)
            if (c == SEPARATEURS[j])
            {
                est_separateur = 1;
                break;
            }

        if (est_separateur || predictionMode)
        {
            // Permet d'éviter de créer des mots vides
            if (i > 0)
            {
                buffer[i] = '\0';

                sequence_addWord(buffer, ht);
                sequence_progress();
                searchOrCreateTreeNode();
                fprintf(fichierSortie, "%s\n", buffer);

                i = 0;
            }

            if (predictionMode)
            {
                TreeNode *noeudFinal = searchOrCreateLeaf(racine, buffer);
                TreeNode *motPrevu = maxOccurrence(noeudFinal);
                if (motPrevu == NULL)
                {
                    fprintf(stderr, COLOR_RED "Erreur : Aucun mot prédit trouvé.\n" COLOR_WHITE);
                    predictionMode = 0;
                    continue;
                }

                printf(COLOR_ORANGE "Mot prédit : " COLOR_WHITE "%s\n", motPrevu->mot);
                printf(COLOR_ORANGE "Acceptez le mot (Entrée) ou saisissez un autre mot : " COLOR_WHITE);
                char userInput[256];
                fgets(userInput, 256, stdin);

                // Mot accepté
                if (userInput[0] == '\n')
                    sequence_addWord(motPrevu->mot, ht);
                // Mot refusé
                else
                {
                    // Enlève le '\n'
                    userInput[strcspn(userInput, "\n")] = '\0';
                    sequence_addWord(userInput, ht);
                }
                sequence_progress();
                predictionMode = 0;
            }
        }
        else if (i < 255)
            buffer[i++] = c;
        else
        {
            perror("Erreur : dépassement de la taille du buffer");
            exit(EXIT_FAILURE);
        }

        if (c == CHAR_PREDI)
            predictionMode = 1;
    }

    printf(COLOR_ORANGE "Fin de saisie.\n" COLOR_WHITE);
}
