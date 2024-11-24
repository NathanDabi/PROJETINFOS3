// main.c
#include "header.h"
#include <stdio.h>

int main() {
    Graphe graphe;
    char nomFichier[100];


    printf("Veuillez saisir le chemin du fichier contenant les donnees du conseil d'administration : ");
    scanf("%s", nomFichier);


    chargerGraphe(nomFichier, &graphe);


    afficherInfluences(&graphe);


    for (int i = 0; i < graphe.ordre; i++) {
        free(graphe.sommets[i].nom);
        free(graphe.matriceAdj[i]);
    }
    free(graphe.matriceAdj);
    free(graphe.sommets);

    return 0;
}
