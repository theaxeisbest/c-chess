#include <stdio.h>
#include <stdbool.h>


#include <unistd.h>


//documentation for sdl: https://wiki.libsdl.org/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* window;//the window
SDL_Renderer* renderer;//the render target

const Uint16 width = 720;
const Uint16 height = 720;

const Uint16 squareWidth = width / 8;
const Uint16 squareHeight = squareWidth;

int main(int argc, char* args[]){

    //init sdl



    if (SDL_Init(SDL_INIT_EVERYTHING) > 0){
        printf("SDL_Init FAILED. SDL_ERROR: ");
        printf("%s", SDL_GetError());
        printf("\n");
        return -1;
    }

    //init sdl with images

    if (!(IMG_Init(IMG_INIT_PNG))){
        printf("SDL image FAILED. SDL_ERROR%s\n", SDL_GetError());
        return -1;
    }

    //create a window



    window = SDL_CreateWindow("Snake!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);//create the window

    //if faield
    if (window == NULL){
        printf("WINDOW FAILED TO INIT. ERROR: ");
        printf("%s", SDL_GetError());
        printf("\n");
        return -1;
    }

    //create the render target
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* grass = IMG_LoadTexture(renderer, "piece/bB.png");




    //make a event
    SDL_Event event;



    //game loop

    Uint16 mousePosX;
    Uint16 mousePosY;

    while (1){

        usleep(10000);



        //do all the events that occur
        while (SDL_PollEvent(&event)){

            //if you pressed the exit button exit
            switch (event.type){
                case SDL_QUIT:

                    goto quit;
                    break;
                case SDL_MOUSEMOTION:
                    mousePosX = event.button.x;
                    mousePosY = event.button.y;
                    break;

            }
        }

        





        SDL_RenderClear(renderer);

        for (int n = 0; n < 8; n++){
            for (int i = 0; i < 8; i++){

                SDL_Rect area;

                area.x = squareWidth * n;
                area.y = squareHeight * i;
                area.w = squareWidth * (n + 1);
                area.h = squareHeight * (i + 1);

                if ((n + (i % 2)) % 2 == 0)
                    SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
                else
                    SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);

                SDL_RenderFillRect(renderer, &area);
            }
        }

        
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


        SDL_RenderPresent(renderer);

    }
    quit:


    //cleanup
    SDL_DestroyWindow(window);
    //quit
    SDL_Quit();

    return 0;
}
