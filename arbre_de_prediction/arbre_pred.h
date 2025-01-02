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

// Structure représentant un noeud de l'arbre de prédiction
typedef struct
{
    char *mot;          // Mot associé au noeud
    int nb_occurrences; // Nombre d'occurrences du N-gramme et du mot suivant
    TabDynamique *fils; // Tableau dynamique des fils du noeud
} TreeNode;

// Arbre principal (contenant tous les mots)
extern TreeNode root;
// Taille initiales des tableaux dynamiques
#define SIZE_TABD 5

/**
 * Initialise l'arbre de prédiction en définissant la racine comme un noeud vide.
 * La racine n'a pas de mot associé, zéro occurrences, et un tableau dynamique vide pour ses fils.
 * Cette fonction doit être appelée avant toute manipulation de l'arbre.
 */
void initArbre();

/**
 * Recherche un chemin correspondant au N-gramme actif dans la table de hachage
 * associée à la séquence actuelle. Si le chemin n'existe pas, les noeuds manquants sont créés.
 *
 * @return Un pointeur vers le dernier noeud du chemin correspondant au N-gramme trouvé ou complété.
 */
TreeNode *searchOrCreateTreeNode();

/**
 * Recherche un fils correspondant au mot donné dans le noeud fourni.
 * Si ce mot n'existe pas, un nouveau noeud feuille est créé avec une occurrence initialisée à 1.
 *
 * @param finalNode Pointeur vers le dernier noeud du N-gramme.
 * @param word Le mot à rechercher ou à ajouter comme feuille.
 * @return Un pointeur vers le noeud feuille contenant le mot (créé ou existant).
 */
TreeNode *searchOrCreateLeaf(TreeNode *finalNode, const char *word);

/**
 * Parcourt les fils du noeud donné pour trouver la feuille contenant le mot ayant
 * le plus grand nombre d'occurrences.
 *
 * @param finalNode Pointeur vers le dernier noeud du chemin du N-gramme.
 * @return Un pointeur vers la feuille contenant le mot avec le maximum d'occurrences. Retourne NULL si le noeud donné n'a pas de fils.
 */
TreeNode *maxOccurrence(TreeNode *finalNode);

void afficherArbre(TreeNode *node, int niveau);
int compterMotsArbre(TreeNode *node);

#endif