/******************************************************************************
 * Fichier : arbre_pred.h
 * Auteur : Emilie Zammit
 * Date de création : 20/11/2024
 * Description : Ce fichier définit les structures et les fonctions nécessaires à la gestion de l'arbre de prédiction des N-grammes. L'arbre de prédiction est utilisé dans le cadre de la prédiction de mots à partir de séquences de N-grammes.
 *****************************************************************************/

#ifndef _ARBRE_PREDICTION_H_
#define _ARBRE_PREDICTION_H_

#include "../tableau_dynamique/tab_dynamique.h"
#include "../sequence/sequence.h"
#include "../sequence/hash.h"

#define SIZE_TABD 5

typedef struct
{
    char *mot;          // Mot associé au noeud
    int nb_occurrences; // Nombre d'occurrences du N-gramme et du mot suivant
    TabDynamique *fils; // Tableau dynamique des fils du noeud
} TreeNode;

extern TreeNode t;

void initArbre();

// Fonction de recherche et de complétion d'un N-gramme dans l'arbre
// Recherche le chemin du N-gramme dans l'arbre et complète si nécessaire.
// Retourne un pointeur vers le dernier noeud du N-gramme trouvé ou complété.
TreeNode *searchOrCreateTreeNode();

TreeNode *searchOrCreateLeaf(TreeNode *finalNode, const char* word);
TreeNode *maxOccurrence(TreeNode *finalNode);

#endif