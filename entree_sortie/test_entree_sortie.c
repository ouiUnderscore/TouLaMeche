/******************************************************************************
 * Fichier : test_entree_sortie.c
 * Auteur : Emilie Zammit
 * Date de création : 27/11/2024
 * Description : Ce fichier permet de tester les fonctions d'entrées sorties.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "entree_sortie.h"
#include "../arbre_de_prediction/arbre_pred.h"
#include "../sequence/sequence.h"

int main()
{
    FILE *fichierSortie = fopen("sortie.txt", "w");
    if (fichierSortie == NULL)
    {
        perror("Erreur d'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    initArbre();
    struct strhash_table *ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);
    
    readKeybord(&root, ht);
    saveTreeNode(fichierSortie, &root, 0);

    fclose(fichierSortie);

    return 0;
}
