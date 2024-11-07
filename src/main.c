/*          Chemin de code pour atteindre ce programme. Attention, il faut se placer à l'intérieur du dossier contenant bin, lib, include et src

gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
bin/prog

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 900

float calculMvmtFusee (float x, float g, float v0, float angle){
    return (-g * x * x)/( 2* (v0 * cosf((M_PI/180)*angle))*(v0 * cosf((M_PI/180)*angle))) + x * tanf((M_PI/180)*angle);
}

float calculVitesseFusee (float x, float g, float v0, float angle){
    return (-g * x) + v0 * sinf((M_PI/180)*angle) + v0 * cosf((M_PI/180)*angle);
}

float calculAngleFusee (float x, float g, float v0, float angle, float p_X)
{
    int calcul = atanf((calculMvmtFusee(x + p_X, g, v0, angle) - calculMvmtFusee(x, g, v0, angle))/p_X)/(M_PI/180);
    if ((calculMvmtFusee(x + p_X, g, v0, angle) - calculMvmtFusee(x, g, v0, angle))/p_X >= 0)
    {
        return 90-calcul;
    }
    else if ((calculMvmtFusee(x + p_X, g, v0, angle) - calculMvmtFusee(x, g, v0, angle))/p_X < 0)
    {
        return 90-calcul;
    }
    
}

int calculLettre(char mot[])
{
    int i = 0;                //On met i à 0
    while (mot[i] != '\0')   //Tant que le mot/phrase n'est pas fini
        i++;                  //on incrémente i de 1
    return i;                 //On retourne i
}

int main(int argc, char *argv[])
{
    SDL_Rect rectangle;                     //Variable rectangle qu'on utilise dans tout le programme pour afficher les textures à des points précis
    
    SDL_Surface *texte = NULL;              //Variable texte qu'on utilise dans tout le programme 

    SDL_Event event;                        //Variable evenement permettant de connaître en temps réel l'evenement qui a lieu (ou pas)

    SDL_Window *window = NULL;              //Création de la fenêtre
    SDL_Renderer *renderer = NULL;          //Création du rendu

    int start = 0;
    char angleC[3], vitesseC[4], distanceC[10], vitesseDC[10];

    int x_angle = 210, y_angle = 60;
    int x_vitesse = 57, y_vitesse = 110;

    int i;                                  //Variable de comptage
    float g = 9.81;                      //Variable du champ de pesanteur
    int angle, v0 = 10;                            //Variable de l'angle initial

    float x = 0;
    float y = 0;
    float p_X = 0.01;

/*------------------------------------------------------------*/

    if(SDL_Init(SDL_INIT_VIDEO) != 0)       //Lancement SDL
        goto finProgramme;

    if(TTF_Init() == -1)                    //Lancement SDl_TTF :
        goto finProgramme;

    TTF_Font *police = NULL;                    //Variable police du jeu
    police = TTF_OpenFont("src/police.ttf", 70);//On sauvegarde l'emplacement de la police dans la variable
    SDL_Color noir = {0,0,0};                   //Définition de la couleur du texte, ici noir


    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0 )  //On créé la fenêtre avec les dimensions 1800*900
        goto finProgramme;

    if(SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)                                    //On détermine la couleur des figures dessinée plus tard 
        goto finProgramme;

    SDL_SetWindowTitle(window, "Modelisation fusée");

/*------------------------------------------------------------------------------------------------------------------------*/  
    SDL_Surface *plan = NULL;                                                      //On initialise une nouvelle surface appelé "plan"
    plan = IMG_Load("src/plan.png");                                             //On charge l'image "Regles" dans la nouvelle surface
    if(plan == NULL)                                                               //On vérifie si l'image a bien été chargée
        goto finProgramme;      //sinon on quitte le programme 
    SDL_Texture *planText = NULL;                                                  //On initialise une nouvelle texture appelé "planText"
    planText = SDL_CreateTextureFromSurface(renderer, plan);                      //On transforme la surface "plan" en une texture sauvegardée dans "planText"
    if(planText == NULL)                                                           //On vérifie si la texture a chargée
        goto finProgramme;   //sinon on quitte le programme
    SDL_FreeSurface(plan);                                                         //On libère la mémoire de la surface "plan" car on n'en a plus besoin


    SDL_Surface *fusee = NULL;                                                      //On initialise une nouvelle surface appelé "fusee"
    fusee = IMG_Load("src/fusee.png");                                             //On charge l'image "Regles" dans la nouvelle surface
    if(fusee == NULL)                                                               //On vérifie si l'image a bien été chargée
        goto finProgramme;      //sinon on quitte le programme 
    SDL_Texture *fuseeText = NULL;                                                  //On initialise une nouvelle texture appelé "fuseeText"
    fuseeText = SDL_CreateTextureFromSurface(renderer, fusee);                      //On transforme la surface "fusee" en une texture sauvegardée dans "fuseeText"
    if(fuseeText == NULL)                                                           //On vérifie si la texture a chargée
        goto finProgramme;   //sinon on quitte le programme
    SDL_FreeSurface(fusee);                                                         //On libère la mémoire de la surface "fusee" car on n'en a plus besoin

    
    SDL_Surface *bouton_glissiere = NULL;                                                      //On initialise une nouvelle surface appelé "bouton_glissiere"
    bouton_glissiere = IMG_Load("src/bouton_glissiere.png");                                             //On charge l'image "Regles" dans la nouvelle surface
    if(bouton_glissiere == NULL)                                                               //On vérifie si l'image a bien été chargée
        goto finProgramme;      //sinon on quitte le programme 
    SDL_Texture *bouton_glissiereText = NULL;                                                  //On initialise une nouvelle texture appelé "bouton_glissiereText"
    bouton_glissiereText = SDL_CreateTextureFromSurface(renderer, bouton_glissiere);                      //On transforme la surface "bouton_glissiere" en une texture sauvegardée dans "bouton_glissiereText"
    if(bouton_glissiereText == NULL)                                                           //On vérifie si la texture a chargée
        goto finProgramme;   //sinon on quitte le programme
    SDL_FreeSurface(bouton_glissiere);                                                         //On libère la mémoire de la surface "bouton_glissiere" car on n'en a plus besoin

    
    SDL_Surface *start_r = NULL;                                                      //On initialise une nouvelle surface appelé "start_r"
    start_r = IMG_Load("src/start_r.png");                                             //On charge l'image "Regles" dans la nouvelle surface
    if(start_r == NULL)                                                               //On vérifie si l'image a bien été chargée
        goto finProgramme;      //sinon on quitte le programme 
    SDL_Texture *start_rText = NULL;                                                  //On initialise une nouvelle texture appelé "start_rText"
    start_rText = SDL_CreateTextureFromSurface(renderer, start_r);                      //On transforme la surface "start_r" en une texture sauvegardée dans "start_rText"
    if(start_rText == NULL)                                                           //On vérifie si la texture a chargée
        goto finProgramme;   //sinon on quitte le programme
    SDL_FreeSurface(start_r);                                                         //On libère la mémoire de la surface "start_r" car on n'en a plus besoin


    SDL_Surface *menu = NULL;                                                      //On initialise une nouvelle surface appelé "menu"
    menu = IMG_Load("src/menu.png");                                             //On charge l'image "Regles" dans la nouvelle surface
    if(menu == NULL)                                                               //On vérifie si l'image a bien été chargée
        goto finProgramme;      //sinon on quitte le programme 
    SDL_Texture *menuText = NULL;                                                  //On initialise une nouvelle texture appelé "menuText"
    menuText = SDL_CreateTextureFromSurface(renderer, menu);                      //On transforme la surface "menu" en une texture sauvegardée dans "menuText"
    if(menuText == NULL)                                                           //On vérifie si la texture a chargée
        goto finProgramme;   //sinon on quitte le programme
    SDL_FreeSurface(menu);                                                         //On libère la mémoire de la surface "menu" car on n'en a plus besoin



    while (1)
    {   
        SDL_Delay(1);
        if (start == 0)
        {    
            angle = (x_angle - 40)/1.89;
            v0 = (x_vitesse - 40)/1.7;
        }
        
        
        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*             Affichage du plan             */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

                                //On défini un rectangle où l'on placera la texture
        rectangle.w = WINDOW_WIDTH;                                               //On défini la largeur de ce que l'on affichera
        rectangle.h = WINDOW_HEIGHT;                                              //On défini la hauteur de ce que l'on affichera
        rectangle.x = 0;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
        rectangle.y = 0;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera

        if(SDL_RenderCopy(renderer, planText, NULL, &rectangle))                               //On copie la texture dans le rendu dans les coordonnées donnée par "rectangle" et on vérifie si ça a marché
            goto finProgramme;   //sinon on quitte

        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*           Affichage de la fusée           */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

                                //On défini un rectangle où l'on placera la texture
        rectangle.w = 100;                                               //On défini la largeur de ce que l'on affichera
        rectangle.h = 143;                                              //On défini la hauteur de ce que l'on affichera
        rectangle.x = 200+100*x;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
        rectangle.y = 607-100*y;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera


        if (start == 0)
        {
            if(SDL_RenderCopyEx(renderer, fuseeText, NULL, &rectangle, 90-angle, NULL, SDL_FLIP_NONE)!=0)                               //On copie la texture dans le rendu dans les coordonnées donnée par "rectangle" et on vérifie si ça a marché
                goto finProgramme;   //sinon on quitte le programme
        }
        else
        {
            if(SDL_RenderCopyEx(renderer, fuseeText, NULL, &rectangle,calculAngleFusee(x, g, v0, 90-angle, p_X), NULL, SDL_FLIP_NONE)!=0)                               //On copie la texture dans le rendu dans les coordonnées donnée par "rectangle" et on vérifie si ça a marché
                goto finProgramme;   //sinon on quitte le programme
        }
        

        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*         Affichage du start rouge          */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

        if (start != 0)
        {
                                //On défini un rectangle où l'on placera la texture
            rectangle.w = 100;                                               //On défini la largeur de ce que l'on affichera
            rectangle.h = 39;                                              //On défini la hauteur de ce que l'on affichera
            rectangle.x = 825;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
            rectangle.y = 796;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera

            if(SDL_RenderCopy(renderer, start_rText, NULL, &rectangle))                               //On copie la texture dans le rendu dans les coordonnées donnée par "rectangle" et on vérifie si ça a marché
                goto finProgramme;   //sinon on quitte
        
        }
        
                              

        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*             Affichage du menu             */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

                                //On défini un rectangle où l'on placera la texture
        rectangle.w = 250;                                               //On défini la largeur de ce que l'on affichera
        rectangle.h = 500;                                              //On défini la hauteur de ce que l'on affichera
        rectangle.x = 10;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
        rectangle.y = 10;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera

        if(SDL_RenderCopy(renderer, menuText, NULL, &rectangle))                               //On copie la texture dans le rendu dans les coordonnées donnée par "rectangle" et on vérifie si ça a marché
            goto finProgramme;   //sinon on quitte


        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*       Affichage du texte de l'angle       */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/ 
        sprintf(angleC, "%i", angle);      

        texte = TTF_RenderText_Blended(police, angleC, noir);
        
        SDL_Texture *textAngle = NULL;
        textAngle = SDL_CreateTextureFromSurface(renderer, texte);
        SDL_FreeSurface(texte);
        if(textAngle == NULL)
            goto finProgramme;
                            //On défini un rectangle où l'on placera la texture
        rectangle.w = 18;
        rectangle.h = 18;
        rectangle.x = 231;
        rectangle.y = 61;

        if(SDL_RenderCopy(renderer, textAngle, NULL, &rectangle))
            goto finProgramme; 
        SDL_DestroyTexture(textAngle);


        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*     Affichage du texte de la vitesse      */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/ 
        sprintf(vitesseC, "%i", v0);

        texte = TTF_RenderText_Blended(police, vitesseC, noir);
        
        SDL_Texture *textVitesse = NULL;
        textVitesse = SDL_CreateTextureFromSurface(renderer, texte);
        SDL_FreeSurface(texte);
        if(textVitesse == NULL)
            goto finProgramme;
                            //On défini un rectangle où l'on placera la texture
        rectangle.w = 18;
        rectangle.h = 18;
        rectangle.x = 231;
        rectangle.y = 111;

        if(SDL_RenderCopy(renderer, textVitesse, NULL, &rectangle))
            goto finProgramme; 
        SDL_DestroyTexture(textVitesse);



        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*     Affichage du texte de la distance     */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/ 
        sprintf(distanceC, "%.2f m", x);
        

        texte = TTF_RenderText_Blended(police, distanceC, noir);
        
        SDL_Texture *textDistance = NULL;
        textDistance = SDL_CreateTextureFromSurface(renderer, texte);
        SDL_FreeSurface(texte);
        if(textDistance == NULL)
            goto finProgramme;
                            //On défini un rectangle où l'on placera la texture
        rectangle.w = 9*calculLettre(distanceC);
        rectangle.h = 18;
        rectangle.x = 875 - rectangle.w/2;
        rectangle.y = 850;

        if(SDL_RenderCopy(renderer, textDistance, NULL, &rectangle))
            goto finProgramme; 
        SDL_DestroyTexture(textDistance);



        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*         Affichage  de la vitesse          */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/ 
        sprintf(vitesseDC, "%.2f m/s", calculVitesseFusee(x, g, v0, angle));
        

        texte = TTF_RenderText_Blended(police, vitesseDC, noir);
        
        SDL_Texture *textVitesseD = NULL;
        textVitesseD = SDL_CreateTextureFromSurface(renderer, texte);
        SDL_FreeSurface(texte);
        if(textVitesseD == NULL)
            goto finProgramme;
                            //On défini un rectangle où l'on placera la texture
        rectangle.w = 9*calculLettre(vitesseDC);
        rectangle.h = 18;
        rectangle.x = 875 - rectangle.w/2;
        rectangle.y = 868;

        if(SDL_RenderCopy(renderer, textVitesseD, NULL, &rectangle))
            goto finProgramme; 
        SDL_DestroyTexture(textVitesseD);


        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*             Affichage du menu             */
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

                                //On défini un rectangle où l'on placera la texture
        rectangle.w = 10;                                               //On défini la largeur de ce que l'on affichera
        rectangle.h = 20;                                              //On défini la hauteur de ce que l'on affichera
        rectangle.x = x_angle;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
        rectangle.y = y_angle;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera

        if(SDL_RenderCopy(renderer, bouton_glissiereText, NULL, &rectangle))                               //On copie la texture dans le rendu dans les coordonnées donnée par "rectangle" et on vérifie si ça a marché
            goto finProgramme;   //sinon on quitte

                                //On défini un rectangle où l'on placera la texture
        rectangle.w = 10;                                               //On défini la largeur de ce que l'on affichera
        rectangle.h = 20;                                              //On défini la hauteur de ce que l'on affichera
        rectangle.x = x_vitesse;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
        rectangle.y = y_vitesse;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera

        if(SDL_RenderCopy(renderer, bouton_glissiereText, NULL, &rectangle))                               //On copie la texture dans le rendu dans les coordonnées donnée par "rectangle" et on vérifie si ça a marché
            goto finProgramme;   //sinon on quitte


        /*-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-*/
        /*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/

        if (start == 1 && y >= 0)
        {
            x += p_X;
            y = calculMvmtFusee(x, g, v0, angle);
        }
        if (start == 1 && y < 0)
        {
            start = 2;
        }
        

        while(SDL_PollEvent(&event)) 
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN :
                    if (event.motion.x > 825 && event.motion.x < 925 && event.motion.y > 796 && event.motion.y < 835)
                    {
                        if (start == 0)
                        {
                            start = 1;
                        }
                        else if (start != 0)
                        {
                            start = 0;
                            x = 0;
                            y = 0;
                        }
                    }
                    break;

                case SDL_MOUSEMOTION :
                    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        if (event.motion.x > x_angle-2 && event.motion.x < x_angle+12 && event.motion.y > y_angle-2 && event.motion.y < y_angle+22)
                        {
                            if (event.motion.x > 46 && event.motion.x < 215 && start == 0)
                                x_angle = event.motion.x-5;
                        }
                        else if (event.motion.x > x_vitesse-2 && event.motion.x < x_vitesse+12 && event.motion.y > y_vitesse-2 && event.motion.y < y_vitesse+22)
                        {
                            if (event.motion.x > 46 && event.motion.x < 215 && start == 0)
                                x_vitesse = event.motion.x-5;
                        }
                    }
                    break;

                case SDL_QUIT :                                 //Si on quitte la fenêtre, on ferme le programme
                    exit(1);
                    break;

                default :
                    break;
            }
        }
        SDL_RenderPresent(renderer);
    }

/*------------------------------------------------------------------------------------------------------------------------*/ 

finProgramme:
    SDL_DestroyTexture(fuseeText);
    SDL_DestroyTexture(start_rText);
    SDL_DestroyTexture(planText);
    SDL_DestroyTexture(bouton_glissiereText);
    SDL_DestroyTexture(menuText);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}