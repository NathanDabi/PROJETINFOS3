// graphe.h
#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdlib.h>
#include <string.h>


typedef struct {
    char* nom;
    int numero;
} Sommet;


Sommet creerSommet(const char* nom, int numero);


typedef struct {
    int** matriceAdj;
    int ordre;
    Sommet* sommets;
} Graphe;


void initGraphe(int ordre, Graphe* graphe);


void chargerGraphe(const char* nomFichier, Graphe* graphe);


void afficherInfluences(const Graphe* graphe);

#endif
