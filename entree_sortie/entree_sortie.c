#include <stdio.h>
#include <string.h>
#include "entree_sortie.h"

// Codes de couleur ANSI
#define COLOR_WHITE "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_ORANGE "\033[33m"

#define BUFFER_SIZE_LINE 1024
#define BUFFER_SIZE_WORD 256

void readKeybord(TreeNode *racine, struct strhash_table *ht)
{
    char c;
    char buffer[BUFFER_SIZE_WORD];
    int i = 0;
    int predictionMode = 0;

    while ((c = getchar()) != EOF)
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
                char userInput[BUFFER_SIZE_WORD];
                fgets(userInput, BUFFER_SIZE_WORD, stdin);

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
        else if (i < BUFFER_SIZE_WORD - 1)
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

// Fonction pour afficher les tabulations en fonction du niveau
void printTabs(FILE *fichier, int level)
{
    for (int i = 0; i < level; i++)
        fprintf(fichier, "\t");
}

void saveTreeNode(FILE *fichier, TreeNode *node, int level)
{
    if (node == NULL)
        return;

    printTabs(fichier, level);
    fprintf(fichier, "[\n");

    printTabs(fichier, level + 1);
    if (node->fils->size == 0)
        fprintf(fichier, "[ p=%d --> %s ]\n", node->nb_occurrences, node->mot);
    else
        fprintf(fichier, "%s\n", node->mot);

    for (int i = 0; i < node->fils->size; i++)
    {
        TreeNode *enfant = (TreeNode *)readTabD(node->fils, i);
        saveTreeNode(fichier, enfant, level + 1);
    }

    printTabs(fichier, level);
    fprintf(fichier, "]\n");
}

void loadTreeNode(FILE *fichier, TreeNode *node)
{
    char line[BUFFER_SIZE];
    int level = -1;
    char buffer[Lg_N_gramme][BUFFER_SIZE_WORD];
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);

    while (fgets(line, sizeof(line), fichier))
    {
        // Retirer le '\n' et les tabs de la ligne
        line[strcspn(line, "\n")] = '\0';
        while (line[0] == '\t')
            memmove(line, line + 1, strlen(line));

        if (strlen(line) == 1 && line[0] == '[')
            level++;
        else if (strlen(line) == 1 && line[0] == ']')
            level--;
        else // Si on est sur une ligne avec un mot
        {
            const char *content = line;
            if (level == Lg_N_gramme)
            {
                char mot[256];
                int occurrences;
                sscanf(content, "[ p=%d --> %255[^]]]", &occurrences, mot);
                strncpy(buffer[level], mot, BUFFER_SIZE_WORD - 1);

                sequence_initialize(ht);
                sequence_addWord(buffer[level - 2], ht);
                sequence_progress();
                sequence_addWord(buffer[level - 1], ht);
                sequence_progress();
                sequence_addWord(buffer[level], ht);
                sequence_progress();

                for (int i = 0; i < occurrences; i++)
                    searchOrCreateTreeNode();
            }
            else
            {
                if (strlen(content) > 0)
                    strncpy(buffer[level], content, BUFFER_SIZE_WORD - 1);
                else
                    strncpy(buffer[level], "", BUFFER_SIZE_WORD - 1);
            }
        }
    }
}

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