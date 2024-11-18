//Adel KHEYAR Léonard BRAULT

#include "Graphe.h"

#define INFINITY INT_MAX

Graphe* lire_graphe(char* nomFichier){
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL){
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    int ordre;
    fscanf(fichier, "%d\n", &ordre);  //lecture de l'ordre
    printf("Ordre du graphe: %d\n", ordre);
    //création du graphe
    Graphe* graphe = CreerGraphe(ordre);
    char* noms_sommets = (char*)malloc(ordre * sizeof(char));

    //lecture des noms des sommet
    for (int i = 0; i < ordre; i++){
        fscanf(fichier, " %c", &noms_sommets[i]);
        printf("Sommet %d: %c\n", i, noms_sommets[i]);
    }
    //lecture de la matrice
    int capacite;
    for (int i = 0; i < ordre; i++){
        for (int j = 0; j < ordre; j++){
            fscanf(fichier, "%d", &capacite);
            if (capacite > 0) {
                graphe->pSommet = CreerArete(graphe->pSommet, i, j, capacite);
            }
        }
    }

    fclose(fichier);
    free(noms_sommets);
    return graphe;
}


pSommet* CreerArete(pSommet* sommet, int s1, int s2, int poids)
{
    //création de l'arc de s1 vers s2
    pArc Newarc1 = (pArc)malloc(sizeof(struct Arc));
    Newarc1->sommet = s2;
    Newarc1->valeur = poids;
    Newarc1->arc_suivant = sommet[s1]->arc;
    sommet[s1]->arc = Newarc1;

    //création de l'arc de s2 vers s1 (car non orienté)
    pArc Newarc2 = (pArc)malloc(sizeof(struct Arc));
    Newarc2->sommet = s1;
    Newarc2->valeur = poids;
    Newarc2->arc_suivant = sommet[s2]->arc;
    sommet[s2]->arc = Newarc2;

    return sommet;
}


Graphe* CreerGraphe(int ordre)
{
    Graphe *Newgraphe = (Graphe*)malloc(sizeof(Graphe));
    if (Newgraphe == NULL){
        printf("Erreur allocation mémoire du graphe\n");
        exit(EXIT_FAILURE);
    }

    Newgraphe->ordre = ordre;
    Newgraphe->taille = 0;
    Newgraphe->pSommet = (pSommet*)malloc(ordre * sizeof(pSommet));

    if (Newgraphe->pSommet == NULL){
        printf("Erreur allocation mémoire des sommets.\n");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < ordre; i++){
        Newgraphe->pSommet[i] = (pSommet)malloc(sizeof(struct Sommet));
        if (Newgraphe->pSommet[i] == NULL) {
            printf("Erreur allocation mémoire pour le sommet %d.\n", i);
            exit(EXIT_FAILURE);
        }
        Newgraphe->pSommet[i]->valeur = i;
        Newgraphe->pSommet[i]->arc = NULL;
    }

    return Newgraphe;
}



bool DFS(int** capaResi, int* chemin, bool* visite, int source, int puit, int ordre) {
    visite[source] = true;

    if (source == puit)
        return true;

    for (int v = 0; v < ordre; v++){
        if (!visite[v] && capaResi[source][v] > 0){
            chemin[v] = source;
            if (DFS(capaResi, chemin, visite, v, puit, ordre))
                return true;
        }
    }

    return false;
}


int felkurson(Graphe* graphe, int source, int puit, int*** flot_retourne){
    int ordre = graphe->ordre;

    //initialise la capacite et la matrice de flot
    int** capaResi = (int**)malloc(ordre * sizeof(int*));
    int** flot = (int**)malloc(ordre * sizeof(int*)); //matrice qui stockerle flot sur chaque arc
    for (int i = 0; i < ordre; i++){
        capaResi[i] = (int*)malloc(ordre * sizeof(int));
        flot[i] = (int*)malloc(ordre* sizeof(int));
        for (int j = 0; j < ordre; j++){
            capaResi[i][j] = 0;
            flot[i][j] = 0;  //initialisation a 0
        }
    }

    for (int u = 0; u < ordre; u++){
        pArc arc = graphe->pSommet[u]->arc;
        while (arc != NULL){
            capaResi[u][arc->sommet] = arc->valeur;
            arc = arc->arc_suivant;
        }
    }

    int* chemin = (int*)malloc(ordre * sizeof(int)); //stocke le chemin augmentant
    bool* visite = (bool*)malloc(ordre * sizeof(bool));
    int flot_max = 0;

    //tant qu'il existe un chemin augmentant dans le graphe
    while (true) {
        for (int i = 0; i < ordre; i++){
            chemin[i] = -1;
            visite[i] = false;
        }

        //exécuter DFS pour trouver un chemin augmentant
        if (!DFS(capaResi, chemin, visite, source, puit, ordre))
            break; //si pas de chemin augmentant on stop

        //trouver la capacité minimale sur le chemin trouvé
        int flot_chemin = INT_MAX;
        for (int v = puit; v != source; v = chemin[v]){
            int u = chemin[v];
            if (flot_chemin > capaResi[u][v]) {
                flot_chemin = capaResi[u][v];
            }
        }

        for (int v = puit; v != source; v = chemin[v]){
            int u = chemin[v];
            capaResi[u][v] -= flot_chemin;
            capaResi[v][u] += flot_chemin;

            //nouvelle matrice des flots
            flot[u][v] += flot_chemin;
            flot[v][u] -= flot_chemin;
        }

        //incrémenter le flot total
        flot_max += flot_chemin;
    }

    for (int i = 0; i < ordre; i++){
        free(capaResi[i]);
    }
    free(capaResi);
    free(chemin);
    free(visite);
    printf("Flot maximal : %d\n", flot_max);
    //retourner la matrice de flots
    *flot_retourne = flot;

    return flot_max;
}



int** getEcart(Graphe* graphe, int** flot){
    int ordre = graphe->ordre;

    int** ecart = (int**)malloc(ordre * sizeof(int*));
    for (int i = 0; i < ordre; i++) {
        ecart[i] = (int*)malloc(ordre * sizeof(int));
    }

    //parcourir les arcs du graphe et calculer l'écart
    for (int u = 0; u < ordre; u++){
        pArc arc = graphe->pSommet[u]->arc;
        while (arc != NULL) {
            int v = arc->sommet;
            //ecart = capacité maximale - flot
            ecart[u][v] = arc->valeur - flot[u][v];
            arc = arc->arc_suivant;
        }
    }

    return ecart;
}

//fonction pour afficher la matrice des écarts
void afficher_ecart(int** ecart, int ordre){
    printf("Matrice des ecarts :\n");
    for (int i = 0; i < ordre; i++) {
        for (int j = 0; j < ordre; j++) {
            printf("%d ", ecart[i][j]);
        }
        printf("\n");
    }
}









int main() {

    Graphe* graphe = lire_graphe("Graphe.txt");

    int source = 0;
    int puit = 6;
    int** flot;
    int flot_max = felkurson(graphe, source, puit, &flot);
    int** ecart = getEcart(graphe, flot);
    afficher_ecart(ecart, graphe->ordre);

    for (int i = 0; i < graphe->ordre; i++){
        pArc arc = graphe->pSommet[i]->arc;
        while (arc != NULL){
            pArc temp = arc;
            arc = arc->arc_suivant;
            free(temp);
        }
        free(graphe->pSommet[i]);
        free(flot[i]);
        free(ecart[i]);
    }
    free(graphe->pSommet);
    free(graphe);

    return 0;

}

