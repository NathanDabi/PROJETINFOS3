//
// Created by sacha on 29/03/2024.
//
#include "header.h"


void item(struct Joueur show, struct Joueur show2, struct item *couteau, struct item *mars)
        {

    int tolerance = 15;



    if (abs(couteau->xc - show.x) <= tolerance && abs(couteau->yc - show.y) <= tolerance && key[KEY_Q]) {
        couteau->xc = show.x;
        couteau->yc = show.y;
    }

    if (abs(mars->xc - show.x) <= tolerance && abs(mars->yc - show.y) <= tolerance && key[KEY_Q]) {
        mars->xc = show.x;
        mars->yc = show.y;
    }

    if (abs(couteau->xc - show2.x) <= tolerance && abs(couteau->yc - show2.y) <= tolerance && key[KEY_M]) {
        couteau->xc = show2.x;
        couteau->yc = show2.y;
    }

    if (abs(mars->xc - show2.x) <= tolerance && abs(mars->yc - show2.y) <= tolerance && key[KEY_M]) {
        mars->xc = show2.x;
        mars->yc = show2.y;
    }
}