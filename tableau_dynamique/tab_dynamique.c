#include <stdio.h>
#include <stdlib.h>
#include "tab_dynamique.h"

TabDynamique *createTabD(int sizeMax)
{
    TabDynamique *t = (TabDynamique *)malloc(sizeof(TabDynamique));
    if (t == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    t->tab = (void **)malloc(sizeMax * sizeof(void *));

    if (t->tab == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    t->size = 0;
    t->sizeMax = sizeMax;

    return t;
}

int freeTabD(TabDynamique *t)
{
    if (t == NULL)
        return 0;

    free(t->tab);
    free(t);

    return 1;
}

int realloueTabD(TabDynamique *t)
{
    if (t == NULL)
        return 0;

    int newSizeMax = t->sizeMax * 2;

    void **newTab = realloc(t->tab, newSizeMax * sizeof(void *));

    if (newTab == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    t->sizeMax = newSizeMax;
    t->tab = newTab;

    return 1;
}

void *readTabD(TabDynamique *t, int position)
{
    if (position >= 0 && position < t->size)
        return t->tab[position];
    else
        return NULL;
}

int fillTabD(TabDynamique *t, int position, void *data)
{
    if (t == NULL || t->tab == NULL)
    {
        fprintf(stderr, "Erreur : tableau ou pointeur NULL\n");
        return 0;
    }

    if (position >= 0 && position < t->sizeMax)
    {
        if (t->tab[position] == NULL)
            t->size++;

        t->tab[position] = data;
        return 1;
    }
    else
    {
        fprintf(stderr, "Erreur : position hors limites\n");
        return 0;
    }
}

int addElemTabD(TabDynamique *t, void *data)
{
    // Si le tableau est rempli, réallouer l'espace pour l'agrandir
    if (t->size == t->sizeMax)
        if (!realloueTabD(t))
        { // Si la réallocation échoue
            fprintf(stderr, "Erreur de réallocation\n");
            return 0;
        }

    t->tab[t->size] = data;
    t->size++;

    return 1;
}

int insertElemTabD(TabDynamique *t, int position, void *data)
{
    if (position < 0 || position > t->sizeMax)
        return 0;

    if (t->size + 1 >= t->sizeMax)
        if (!realloueTabD(t))
        {
            // Si la réallocation échoue
            fprintf(stderr, "Erreur de réallocation\n");
            return 0;
        }

    for (int i = t->size - 1; i >= position; i--)
        t->tab[i + 1] = t->tab[i];

    t->tab[position] = data;
    t->size++;

    return 1;
}

int rechercheElemTabD(TabDynamique *t, void *data, int (*compare)(void *, void *))
{
    for (int i = 0; i < t->size; i++)
        if ((compare(t->tab[i], data)) == 0)
            return i;

    return 0;
}