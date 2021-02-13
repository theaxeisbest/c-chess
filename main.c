#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>


//documentation for sdl: https://wiki.libsdl.org/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>

#include "squares.h"
#include "moves.h"
#include "engine.h"

SDL_Window* window;//the window
SDL_Renderer* renderer;//the render target

const Uint16 width = 720;
const Uint16 height = 720;

const Uint16 squareWidth = width / 8;
const Uint16 squareHeight = squareWidth;

SDL_Texture* textures[12];

char board[64] = {BLACK_ROCK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROCK,
                    BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN,
                    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                    WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN,
                    WHITE_ROCK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROCK
                    };

void* DisplayWinMessage(void* whoWon){
    const bool wonWhite = *(char*)whoWon;

    #ifndef _WIN32
    //TODO: inplement this for other linux systems with GTK+  
    if (wonWhite)
        system("zenity --info --text \"You won!\"");
    else
        system("zenity --info --text \"yThe computer won!\"");

    #else
    //TODO: inplement windows dialoge box
    #endif

    return NULL;
}

int main(int argc, char* args[]){

    //init sdl

    {

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

    window = SDL_CreateWindow("Chess!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);//create the window

    
    if (window == NULL){
        printf("WINDOW FAILED TO INIT. ERROR: ");
        printf("%s", SDL_GetError());
        printf("\n");
        return -1;
    }

    //create the render target
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    }

    //load textures
    {
    
    textures[BLACK_BISHOP] = IMG_LoadTexture(renderer, "piece/bB.png");
    textures[BLACK_KING] = IMG_LoadTexture(renderer, "piece/bK.png");
    textures[BLACK_KNIGHT] = IMG_LoadTexture(renderer, "piece/bN.png");
    textures[BLACK_PAWN] = IMG_LoadTexture(renderer, "piece/bP.png");
    textures[BLACK_QUEEN] = IMG_LoadTexture(renderer, "piece/bQ.png");
    textures[BLACK_ROCK] = IMG_LoadTexture(renderer, "piece/bR.png");
    textures[WHITE_BISHOP] = IMG_LoadTexture(renderer, "piece/wB.png");
    textures[WHITE_KING] = IMG_LoadTexture(renderer, "piece/wK.png");
    textures[WHITE_KNIGHT] = IMG_LoadTexture(renderer, "piece/wN.png");
    textures[WHITE_PAWN] = IMG_LoadTexture(renderer, "piece/wP.png");
    textures[WHITE_QUEEN] = IMG_LoadTexture(renderer, "piece/wQ.png");
    textures[WHITE_ROCK] = IMG_LoadTexture(renderer, "piece/wR.png");
    
    
    }
    
	char selected = -1;
    bool blacksTurn = false;

    SDL_Event event;

    Uint16 mousePosX;
    Uint16 mousePosY;

    while (1){

        usleep(10000);
        
        {
        bool blacksKing = false;
        bool whitesKing = false;
        for (char n = 0; n < 64; n++){
            if (board[n] == WHITE_KING){
                whitesKing = true;
            } else if (board[n] == BLACK_KING){
                blacksKing = true;
            }
        }

        if (!(blacksKing && whitesKing)){
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, DisplayWinMessage, &whitesKing);
            return 0;
        }

        }

        {
        bool blacksKing = false;
        bool whitesKing = false;
        for (char n = 0; n < 64; n++){
            if (board[n] == WHITE_KING){
                whitesKing = true;
            } else if (board[n] == BLACK_KING){
                blacksKing = true;
            }
        }

        }

        //computers turn
        if (blacksTurn){
            char* move = MakeBestMove(board);

            BoardAfterMove(board, move[0], move[1]);

            blacksTurn = false;

        }

        //if won


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
				case SDL_MOUSEBUTTONDOWN:
					
					if (selected != -1){

                        const char destination = (mousePosX / squareWidth) + (mousePosY / squareHeight) * 8;

                        if (IsMoveLegal(selected, destination, board)){
                            BoardAfterMove(board, selected, destination);
                            blacksTurn = !blacksTurn;
                        }

						selected = -1;
						break;
					}

					selected = (mousePosX / squareWidth) + (mousePosY / squareHeight) * 8;

                    if (board[selected] < EMPTY){
                        selected = -1;
                    }


					break;

            }
        }

        





        SDL_RenderClear(renderer);

        for (int n = 0; n < 8; n++){
            for (int i = 0; i < 8; i++){
                char boardIndex = i * 8 + n;

                SDL_Rect area;

                area.x = squareWidth * n;
                area.y = squareHeight * i;
                area.w = 90;
                area.h = 90;

				if (boardIndex == selected){
					SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
				}else if ((n + (i % 2)) % 2 == 1)
                    SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
                else
                    SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);

                SDL_RenderFillRect(renderer, &area);


				if (board[boardIndex] == EMPTY) continue;

    	        SDL_RenderCopy(renderer, textures[board[boardIndex]], NULL, &area);

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
