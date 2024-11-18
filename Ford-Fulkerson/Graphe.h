#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

struct Arc {
    int sommet;
    int valeur;
    struct Arc* arc_suivant;
};

typedef struct Arc* pArc;

struct Sommet {
    struct Arc* arc;
    int valeur;
};

typedef struct Sommet* pSommet;

typedef struct Graphe {
    int taille;
    int orientation;
    int ordre;
    pSommet* pSommet;
} Graphe;


Graphe* CreerGraphe(int ordre);

pSommet* CreerArete(pSommet* sommet, int s1, int s2, int poids);

Graphe* lire_graphe(char* nomFichier);

void afficher_successeurs(pSommet* sommet, int num);

int felkurson(Graphe* graphe, int source, int destination, int*** flot_retourne);

bool DFS(int** capacite_residuelle, int* chemin, bool* visite, int u, int destination, int ordre);

int** getEcart(Graphe* graphe, int** flot);

void afficher_ecart(int** ecart, int ordre);

void liberer_graphe(Graphe* graphe);

#endif // GRAPHE_H_INCLUDED
