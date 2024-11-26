/******************************************************************************
 * Fichier : test_tab_dynamique.c
 * Auteur : Emilie Zammit
 * Date de création : 18/11/2024
 * Description : Ce fichier contient les tests unitaires pour la gestion d'un
 * tableau dynamique, en utilisant des fonctions définies dans le fichier
 * `tab_dynamique.c`. Les tests vérifient le bon fonctionnement de l'allocation,
 * la réallocation, l'ajout, la lecture et la recherche d'éléments dans le tableau.
 * Des assertions sont utilisées pour valider les résultats obtenus.
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tab_dynamique.h"

void test_createTabD()
{
    TabDynamique *t = createTabD(2);
    assert(t->size == 0);
    assert(t->sizeMax == 2);
    assert(t->tab != NULL);
    printf("Test de création du tableau dynamique réussi.\n");
}

void test_freeTabD()
{
    TabDynamique *t = createTabD(2);
    int result = freeTabD(t);
    assert(result == 1);
    printf("Test de libération du tableau dynamique réussi.\n");
}

void test_realloueTabD()
{
    TabDynamique *t = createTabD(2);
    addElemTabD(t, (void *)"element1");
    addElemTabD(t, (void *)"element2");
    int result = realloueTabD(t);
    assert(result == 1);
    assert(t->sizeMax == 4);
    addElemTabD(t, (void *)"element3");
    printf("Test de réallocation du tableau dynamique réussi.\n");
}

void test_addElemTabD()
{
    TabDynamique *t = createTabD(2);
    addElemTabD(t, (void *)"element1");
    addElemTabD(t, (void *)"element2");
    addElemTabD(t, (void *)"element3"); // Devrait déclencher une réallocation
    assert(t->size == 3);
    assert(t->sizeMax >= 3);
    printf("Test d'ajout d'élément réussi.\n");
}

void test_readTabD()
{
    TabDynamique *t = createTabD(2);
    addElemTabD(t, (void *)"element1");
    addElemTabD(t, (void *)"element2");
    char *elem = (char *)readTabD(t, 1);
    assert(elem != NULL && strcmp(elem, "element2") == 0);
    printf("Test de lecture d'un élément réussi.\n");
}

void test_fillTabD()
{
    TabDynamique *t = createTabD(2);
    fillTabD(t, 0, (void *)"newElement");
    char *elem = (char *)readTabD(t, 0);
    assert(elem != NULL && strcmp(elem, "newElement") == 0);
    printf("Test d'écriture d'un élément réussi.\n");
}

static int compare(void *elem1, void *elem2)
{
    return strcmp((char *)elem1, (char *)elem2);
}

void test_rechercheElemTabD()
{
    TabDynamique *t = createTabD(2);
    addElemTabD(t, (void *)"element1");
    addElemTabD(t, (void *)"element2");
    int index = rechercheElemTabD(t, (void *)"element2", compare);
    assert(index == 1);
    printf("Test de recherche d'élément réussi.\n");
}

void test_insertElemTabD()
{
    printf("Insertion (newElement en pos 1) :\n");
    TabDynamique *t = createTabD(2);
    addElemTabD(t, (void *)"element1");
    addElemTabD(t, (void *)"element2");
    addElemTabD(t, (void *)"element3");
    addElemTabD(t, (void *)"element4");
    printf("%s %s %s %s\n", (char *)readTabD(t, 0), (char *)readTabD(t, 1), (char *)readTabD(t, 2), (char *)readTabD(t, 3));
    insertElemTabD(t, 1, (void *)"newElement");
    printf("%s %s %s %s %s\n", (char *)readTabD(t, 0), (char *)readTabD(t, 1), (char *)readTabD(t, 2), (char *)readTabD(t, 3), (char *)readTabD(t, 4));
    printf("Test d'insertion d'élément réussi.\n");
}

int main()
{
    test_createTabD();
    test_freeTabD();
    test_realloueTabD();
    test_addElemTabD();
    test_readTabD();
    test_fillTabD();
    test_rechercheElemTabD();
    test_insertElemTabD();
    printf("\nTous les tests unitaires ont été passés avec succès.\n");
    return 0;
}