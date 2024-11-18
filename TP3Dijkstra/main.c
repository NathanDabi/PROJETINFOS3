#include <stdio.h>
#include "Graphe.h"
#include <limits.h>

#define INFINITY INT_MAX



//compteur de sommets explorés
int sommets_explores_dijkstra = 0;
int sommets_explores_astar = 0;

// fonction heuristique
int heuristique(Graphe* graphe, int sommet_actuel, int sommet_arrive)
{
    // heuristique proposée : poids minimal des arêtes * nombre d'arêtes restantes
    int min_poids = INT_MAX;
    pArc arc = graphe->pSommet[sommet_actuel]->arc;
    while (arc != NULL)
    {
        if (arc->valeur < min_poids)
        {
            min_poids = arc->valeur;
        }
        arc = arc->arc_suivant;
    }
    // ici, on suppose que la distance heuristique est égale au poids minimal
    return min_poids;
}



//fonction pour afficher le plus court chemin
void afficher_chemin(int *predecesseur, int sommet_depart, int sommet_arrive)
{
    if (sommet_arrive == -1) {
        return;
    }
    afficher_chemin(predecesseur, sommet_depart, predecesseur[sommet_arrive]);
    if (predecesseur[sommet_arrive] != -1) {
        printf("->");
    }
    printf("%d", sommet_arrive);
}

// algorithme A*
void astar(Graphe* graphe, int sommet_depart, int sommet_arrive)
{
    int ordre = graphe->ordre;
    int distance[ordre], f_cost[ordre];
    int marque[ordre];
    int predecesseur[ordre];

    // Initialisation
    for (int i = 0; i < ordre; i++) {
        distance[i] = INT_MAX;
        f_cost[i] = INT_MAX;
        marque[i] = 0;
        predecesseur[i] = -1;
    }
    distance[sommet_depart] = 0;
    f_cost[sommet_depart] = heuristique(graphe, sommet_depart, sommet_arrive);

    for (int i = 0; i < ordre; i++) {
        int min_fcost = INT_MAX;
        int min_sommet = -1;

        // Trouver le sommet non marqué avec le plus petit f_cost
        for (int j = 0; j < ordre; j++) {
            if (!marque[j] && f_cost[j] < min_fcost) {
                min_fcost = f_cost[j];
                min_sommet = j;
            }
        }

        if (min_sommet == -1) {
            break;
        }

        marque[min_sommet] = 1;
        sommets_explores_astar++;  // Incrément du compteur de sommets explorés

        if (min_sommet == sommet_arrive) {
            break;  // Chemin trouvé
        }

        // Explorer les successeurs
        pArc arc = graphe->pSommet[min_sommet]->arc;
        while (arc != NULL) {
            int successeur = arc->sommet;
            int poids = arc->valeur;

            if (!marque[successeur] && distance[min_sommet] + poids < distance[successeur]) {
                distance[successeur] = distance[min_sommet] + poids;
                f_cost[successeur] = distance[successeur] + heuristique(graphe, successeur, sommet_arrive);
                predecesseur[successeur] = min_sommet;
            }
            arc = arc->arc_suivant;
        }
    }

    if (distance[sommet_arrive] == INT_MAX) {
        printf("Aucun chemin trouve entre %d et %d avec A*.\n", sommet_depart, sommet_arrive);
    } else {
        printf("Avec A*, entre les sommets %d et %d, le plus court chemin est ", sommet_depart, sommet_arrive);
        afficher_chemin(predecesseur, sommet_depart, sommet_arrive);
        printf(" de longueur %d.\n", distance[sommet_arrive]);
    }
}




//affichage des successeurs du sommet
void afficher_successeurs(pSommet * sommet, int num)
{
    printf(" sommet %d :\n", num);
    pArc arc = sommet[num]->arc;

    while (arc != NULL)
    {
        printf("%d (poids = %d) ", arc->sommet, arc->valeur);
        arc = arc->arc_suivant;
    }
    printf("\n");
}

//ajouter l'arête entre les sommets s1 et s2
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


Graphe* lire_graphe(char* nomFichier)
{
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    int ordre, taille, s1, s2, poids;
    fscanf(fichier, "%d", &ordre);
    Graphe* graphe = CreerGraphe(ordre);

    fscanf(fichier, "%d", &graphe->orientation);


    for (int i = 0; i < ordre; i++) {
        int sommet;
        fscanf(fichier, "%d", &sommet);
    }

    fscanf(fichier, "%d", &taille);

    //lecture des arêtes
    for (int i = 0; i < taille; i++) {
        fscanf(fichier, "%d %d %d", &s1, &s2, &poids);
        graphe->pSommet = CreerArete(graphe->pSommet, s1, s2, poids);  //ajout des arêtes
        graphe->taille++;  //incrémente la taille du graphe (
    }

    fclose(fichier);
    return graphe;
}








//algorithme de Dijkstra
void dijkstra(Graphe* graphe, int sommet_depart, int sommet_arrive)
{
    int ordre = graphe->ordre;
    int distance[ordre];
    int marque[ordre];
    int predecesseur[ordre];

    //initialisation
    for (int i = 0; i < ordre; i++) {
        distance[i] = INT_MAX; //distance infinie au début
        marque[i] = 0;         //tous les sommets sont non marqués
        predecesseur[i] = -1;  //aucun prédécesseur au début
    }
    distance[sommet_depart] = 0;

    for (int i = 0; i < ordre; i++) {
        //trouver le sommet non marqué avec la plus petite distance
        int min_dist = INT_MAX;
        int min_sommet = -1;
        for (int j = 0; j < ordre; j++) {
            if (!marque[j] && distance[j] < min_dist) {
                min_dist = distance[j];
                min_sommet = j;
            }
        }

        if (min_sommet == -1) {
            break; //tous les sommets atteignables ont été visités
        }

        marque[min_sommet] = 1;

        pArc arc = graphe->pSommet[min_sommet]->arc;
        while (arc != NULL) {
            int sommet_successeur = arc->sommet;
            int poids = arc->valeur;
            if (!marque[sommet_successeur] && distance[min_sommet] + poids < distance[sommet_successeur]) {
                distance[sommet_successeur] = distance[min_sommet] + poids;
                predecesseur[sommet_successeur] = min_sommet;
            }
            arc = arc->arc_suivant;
        }
    }

    //si aucun chemin trouvé
    if (distance[sommet_arrive] == INT_MAX) {
        printf("Aucun chemin trouve entre %d et %d\n", sommet_depart, sommet_arrive);
    } else {
        //affichage du chemin et de la longueur
        printf("Entre les sommets %d et %d, le plus court chemin est ", sommet_depart, sommet_arrive);
        afficher_chemin(predecesseur, sommet_depart, sommet_arrive);
        printf(" de longueur %d.\n", distance[sommet_arrive]);
    }
}


Graphe* CreerGraphe(int ordre)
{
    Graphe *Newgraphe = (Graphe*)malloc(sizeof(Graphe));
    if (Newgraphe == NULL) {
        printf("Erreur d'allocation mémoire pour le graphe.\n");
        exit(EXIT_FAILURE);
    }

    Newgraphe->ordre = ordre;
    Newgraphe->taille = 0;
    Newgraphe->orientation = 0;
    Newgraphe->pSommet = (pSommet*)malloc(ordre * sizeof(pSommet));

    if (Newgraphe->pSommet == NULL) {
        printf("Erreur d'allocation mémoire pour les sommets.\n");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < ordre; i++) {
        Newgraphe->pSommet[i] = (pSommet)malloc(sizeof(struct Sommet));
        if (Newgraphe->pSommet[i] == NULL) {
            printf("Erreur d'allocation mémoire pour le sommet %d.\n", i);
            exit(EXIT_FAILURE);
        }
        Newgraphe->pSommet[i]->valeur = i;
        Newgraphe->pSommet[i]->arc = NULL;
    }

    return Newgraphe;
}


int main()
{
    char nomFichier[] = "graphe.txt";
    Graphe* graphe = lire_graphe(nomFichier);

    if (graphe == NULL)
    {
        printf("Erreur lors de la lecture du graphe\n");
        return -1;
    }

    int depart, arrivee;
    printf("Entrez le sommet de depart : ");
    scanf("%d", &depart);
    printf("Entrez le sommet d'arrivee : ");
    scanf("%d", &arrivee);

    dijkstra(graphe, depart, arrivee);


    return 0;
}
