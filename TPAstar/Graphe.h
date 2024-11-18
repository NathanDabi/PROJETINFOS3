#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Arc
{
    int sommet;
    int valeur;
    struct Arc* arc_suivant;
};

typedef struct Arc* pArc;

struct Sommet
{
    struct Arc* arc;
    int valeur;
    char couleur;
};

typedef struct Sommet* pSommet;

typedef struct Graphe
{
    int taille;
    int orientation;
    int ordre;
    pSommet* pSommet;
} Graphe;

Graphe* CreerGraphe(int ordre);

pSommet* CreerArete(pSommet* sommet, int s1, int s2, int poids);

void afficher_successeurs(pSommet* sommet, int num);

Graphe* lire_graphe(char* nomFichier);

void afficher_chemin(int* predecesseur, int sommet_depart, int sommet_arrive);

void dijkstra(Graphe* graphe, int sommet_depart, int sommet_arrive, int* sommets_explores_dijkstra);

void astar(Graphe* graphe, int sommet_depart, int sommet_arrive);

int heuristique(Graphe* graphe, int sommet_actuel, int sommet_arrive);

#endif // GRAPHE_H_INCLUDED
