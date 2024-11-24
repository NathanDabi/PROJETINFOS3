// graphe.c
#include "header.h"
#include <stdio.h>


Sommet creerSommet(const char* nom, int numero) {
    Sommet sommet;
    sommet.nom = (char*)malloc((strlen(nom) + 1) * sizeof(char));
    strcpy(sommet.nom, nom);
    sommet.numero = numero;
    return sommet;
}

void initGraphe(int ordre, Graphe* graphe) {
    graphe->ordre = ordre;

    graphe->matriceAdj = (int**)malloc(ordre * sizeof(int*));
    for (int i = 0; i < ordre; i++) {
        graphe->matriceAdj[i] = (int*)malloc(ordre * sizeof(int));
        for (int j = 0; j < ordre; j++) {
            graphe->matriceAdj[i][j] = 0;
        }
    }


    graphe->sommets = (Sommet*)malloc(ordre * sizeof(Sommet));
}


void chargerGraphe(const char* nomFichier, Graphe* graphe) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    int ordre;
    fscanf(fichier, "%d", &ordre);
    initGraphe(ordre, graphe);


    for (int i = 0; i < ordre; i++) {
        char nom[50];
        fscanf(fichier, "%s", nom);
        graphe->sommets[i] = creerSommet(nom, i);
    }

    for (int i = 0; i < ordre; i++) {
        for (int j = 0; j < ordre; j++) {
            fscanf(fichier, "%d", &graphe->matriceAdj[i][j]);
        }
    }

    fclose(fichier);
}


void afficherInfluences(const Graphe* graphe) {
    for (int i = 0; i < graphe->ordre; i++) {
        printf("%s influence : ", graphe->sommets[i].nom);
        int influce = 0;
        for (int j = 0; j < graphe->ordre; j++) {
            if (graphe->matriceAdj[i][j] == 1) {
                printf("%s ", graphe->sommets[j].nom);
                influce = 1;
            }
        }
        if (!influce) {
            printf("personne");
        }
        printf("\n");
    }
}
