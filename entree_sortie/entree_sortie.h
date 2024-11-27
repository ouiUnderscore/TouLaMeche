/******************************************************************************
 * Fichier : entree_sortie.h
 * Auteur : Emilie Zammit
 * Date de création : 27/11/2024
 * Description : Ce fichier définit les structures et les fonctions nécessaires à la gestion des entrées textuelles dans le terminal ainsi qu'à la prédiction et l'affichage.
 *****************************************************************************/

#ifndef _ENTREE_SORTIE_
#define _ENTREE_SORTIE_

#include <stdio.h>
#include "../arbre_de_prediction/arbre_pred.h"
// #define SEPARATEURS " \n.,;:?!-_()[]{}'\"/\\|&@#$%^*+=<>`~"
#define SEPARATEURS " \n.\t,"
#define CHAR_PREDI '\t'
#define CHAR_FIN 27 // esc
#define BUFFER_SIZE 256

// Fonction pour lire un texte depuis l'entrée standard
void readKeybord(TreeNode *racine, struct strhash_table *ht);

// Fonction pour sauvegarder l'arbre dans un fichier
void saveTreeNode(FILE *fichier, TreeNode *node, int level);

// Fonction pour charger l'arbre depuis un fichier
TreeNode *loadTreeNode(FILE *fichier);

#endif