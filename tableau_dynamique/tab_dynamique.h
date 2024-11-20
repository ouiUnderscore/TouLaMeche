/******************************************************************************
 * Fichier : tab_dynamique.c
 * Auteur : Emilie Zammit
 * Date de création : 15/11/2024
 * Description :
 *****************************************************************************/

#ifndef _TAB_DYNAMIQUE_H_
#define _TAB_DYNAMIQUE_H_

#include <stdlib.h>

typedef struct
{
    void **tab;  // Tableau de pointeurs vers les éléments
    int size;    // Nombre d'éléments dans le tableau
    int sizeMax; // Nombre maximum d'éléments alloués
} TabDynamique;

/**
 * @brief Création du tableau dynamique avec une taille maximale spécifiée.
 *
 * Alloue la mémoire nécessaire pour le tableau et initialise la structure.
 * @param t Pointeur vers la structure du tableau dynamique
 * @param sizeMax Taille maximale initiale du tableau
 * @return tableau dynamique initialisé
 */
TabDynamique *createTabD(int sizeMax);

/**
 * @brief Libère la mémoire allouée pour le tableau dynamique.
 *
 * La mémoire allouée pour le tableau est libérée, mais les données référencées par
 * les éléments du tableau ne sont pas libérées.
 * @param t Pointeur vers la structure du tableau dynamique
 * @return 1 si l'allocation est réussie, 0 sinon
 */
int freeTabD(TabDynamique *t);

/**
 * @brief Réalloue le tableau dynamique pour augmenter sa capacité.
 *
 * Cette fonction réalloue la mémoire du tableau pour doubler sa capacité maximale
 * lorsque le tableau est plein. Elle met également à jour la taille maximale dans la structure.
 *
 * @param t Pointeur vers la structure du tableau dynamique
 * @return 1 si l'allocation est réussie, 0 sinon
 */
int realloueTabD(TabDynamique *t);

/**
 * @brief Lecture d'un élément à une position donnée dans le tableau.
 *
 * @param t Pointeur vers la structure du tableau dynamique
 * @param position Position de l'élément à lire
 * @return Pointeur vers l'élément à la position donnée, ou NULL si la position est invalide
 */
void *readTabD(TabDynamique *t, int position);

/**
 * @brief Écriture d'un élément à une position donnée dans le tableau.
 *
 * @param t Pointeur vers la structure du tableau dynamique
 * @param position Position où l'élément doit être écrit
 * @param data Pointeur vers les données à écrire
 * @return 1 si l'écriture a réussi, 0 si la position est invalide
 */
int fillTabD(TabDynamique *t, int position, void *data);

/**
 * @brief Ajoute un élément à la fin du tableau dynamique.
 *
 * Si nécessaire, réalloue de la mémoire pour agrandir le tableau.
 * @param t Pointeur vers la structure du tableau dynamique
 * @param data Pointeur vers les données à ajouter
 * @return 1 si l'ajout a réussi, 0 sinon (en cas d'échec de réallocation)
 */
int addElemTabD(TabDynamique *t, void *data);

/**
 * @brief Insère un élément à une position donnée dans le tableau dynamique.
 *
 * Les éléments suivants sont décalés pour accueillir le nouvel élément.
 * Si nécessaire, réalloue de la mémoire pour agrandir le tableau.
 * @param t Pointeur vers la structure du tableau dynamique
 * @param position Position où l'élément doit être inséré
 * @param data Pointeur vers les données à insérer
 * @return 1 si l'insertion a réussi, 0 sinon (en cas d'échec de réallocation ou position invalide)
 */
int insertElemTabD(TabDynamique *t, int position, void *data);

/**
 * @brief Recherche la position d'un élément dans le tableau dynamique.
 *
 * La recherche est basée sur la comparaison des éléments, à l'aide de la fonction
 * `compare` fournie en paramètre.
 *
 * @param t Pointeur vers la structure du tableau dynamique
 * @param data Pointeur vers les données à rechercher
 * @param compare Pointeur vers la fonction de comparaison utilisée pour comparer les éléments
 * @return L'index de l'élément trouvé, ou -1 si l'élément n'est pas trouvé
 */
int rechercheElemTabD(TabDynamique *t, void *data, int (*compare)(void *, void *));

#endif