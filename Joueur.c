//
// Created by sacha on 28/03/2024.
//


#include "header.h"

int player()
{

    struct Joueur show;
    struct Joueur show2;

    struct plateau back;
    struct item couteau;
    struct item mars;
    struct item jean;


    couteau.xc = 250;
    couteau.yc = 250;

    mars.xc = 350;
    mars.yc = 350;

    // Position initiale du joueur1
    show.x = 100; // Position en x
    show.y = 100; // Position en y


    // Position initiale du joueur2
    show2.x = 500; // Position en x
    show2.y = 500; // Position en y

    // Affichage de la souris
    show_mouse(screen);

    couteau.couteau = load_bitmap("couteau.bmp", NULL);
    show.joueur1 = load_bitmap("Gab.bmp", NULL);
    show2.joueur2 = load_bitmap("hugo.bmp", NULL);
    mars.mars = load_bitmap("mars.bmp", NULL);
    back.fond = load_bitmap("Cuisine.bmp", NULL);
    jean.jean = load_bitmap("jean.bmp", NULL);
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    show.angle =  itofix(64 * 3);
    show2.angle =  itofix(64 * 3);

    // Boucle de jeu
    while (!key[KEY_ESC])
    {

        clear_bitmap(buffer);

        // Déplacement de l'image1 en utilisant les touches Z, Q, S, D
        if (key[KEY_W]) {show.y -= 5; show.angle = itofix(64 * 3);}
        else if (key[KEY_S]) {show.y += 5; show.angle = itofix(0);}
        else if (key[KEY_A]) {show.x -= 5; show.angle = itofix(64 * 2);}
        else if (key[KEY_D]) {show.x += 5; show.angle = itofix(64);}



        // Déplacement de l'image2 en utilisant les touches fleche
        if (key[KEY_UP]) { show2.y -= 5;  show2.angle = itofix(64 * 3);}
        else if (key[KEY_DOWN]) { show2.y += 5; show2.angle = itofix(0);}
        else if (key[KEY_LEFT]) { show2.x -= 5; show2.angle = itofix(64 * 2);}
        else if (key[KEY_RIGHT]) { show2.x += 5; show2.angle = itofix(64);}

    int tolerance = 15;

        item(show, show2, &couteau , &mars);
        limitation(&show, &show2);
        tapi(&show, &show2, &couteau, &mars);


        blit(back.fond, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        blit(show.joueur1, buffer, 0, 0, show.x, show.y, show.joueur1->w, show.joueur1->h);
        blit(show2.joueur2, buffer, 0, 0, show2.x, show2.y, show2.joueur2->w, show2.joueur2->h);


        rotate_sprite(buffer, show.joueur1, show.x, show.y, show.angle);
        rotate_sprite(buffer, show2.joueur2, show2.x, show2.y, show2.angle);


        blit(couteau.couteau, buffer, 0, 0, couteau.xc, couteau.yc, couteau.couteau->w, couteau.couteau->h);
        blit(mars.mars, buffer, 0, 0, mars.xc, mars.yc, mars.mars->w, mars.mars->h);

        if (abs(mars.xc - couteau.xc) <= tolerance && abs(mars.yc - couteau.yc) <= tolerance)
        {
            clear_bitmap(couteau.couteau);
            clear_bitmap(mars.mars);

            blit(jean.jean, buffer, 0, 0, jean.xc, jean.yc, jean.jean->w, jean.jean->h);
            jean.xc = couteau.xc;
            jean.yc = couteau.yc;
        }



        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(5);

    }

    // Libération de la mémoire

    destroy_bitmap(show.joueur1);
    destroy_bitmap(show2.joueur2);
    destroy_bitmap(back.fond);


    return 0;
}END_OF_FUNCTION(player)