#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>


//documentation for sdl: https://wiki.libsdl.org/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "squares.h"

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

void PromatePawn(char pawnIndex){
    if (pawnIndex <= 8){
        board[pawnIndex] = WHITE_QUEEN;
    } else{
        board[pawnIndex] = BLACK_QUEEN;
    }
}

bool IsMoveLegal(char indexPiece, char indexDestination){

    char pieceX, pieceY, destinationX, destinationY;

    pieceX = indexPiece % 8;
    pieceY = indexPiece / 8;

    destinationX = indexDestination % 8;
    destinationY = indexDestination / 8;

    if ((board[indexPiece] <= BLACK_ROCK && board[indexDestination] <= BLACK_ROCK) ||
        (board[indexPiece] >= WHITE_BISHOP && board[indexDestination] >= WHITE_BISHOP)){
        return false;
    }

    switch (board[indexPiece])
    {
    case EMPTY:
        return false;
    case WHITE_BISHOP:
    case BLACK_BISHOP:

        if (pieceX - destinationX == pieceY - destinationY){
            for (char i = pieceX; i != destinationX; i += (destinationX - pieceX) / SDL_abs(destinationX - pieceX)){
                if (i == pieceX) continue;
                if (board[(pieceY + (i - pieceX)) * 8 + i] != EMPTY){
                    return false;
                }
            }
            return true;
        } else if (SDL_abs(pieceX - destinationX) == SDL_abs(pieceY - destinationY)){
            for (char i = pieceX; i != destinationX; i += (destinationX - pieceX) / SDL_abs(destinationX - pieceX)){
                if (i == pieceX) continue;
                if (board[(pieceY - (i - pieceX)) * 8 + i] != EMPTY){
                    return false;
                }
            }
            return true;
        }

        return false;
    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
        ;
        const signed char LegealMoves[] = {-10,6,15,17,10,-6,-15,-17};

        for (char i = 0; i < 8; i++){
            if (indexPiece - indexDestination == LegealMoves[i]){
                return true;
            }

        }

        return false;
    case WHITE_QUEEN:
    case BLACK_QUEEN:

        if (destinationY == pieceY){
            for (char i = pieceX; i != destinationX; i += (destinationX - pieceX) / SDL_abs(destinationX - pieceX)){
                if (i == pieceX) continue;
                if (board[pieceY * 8 + i] != EMPTY){
                    return false;
                }
            }
            return true;
        } else if (destinationX == pieceX){
            for (char i = pieceY; i != destinationY; i += (destinationY - pieceY) / SDL_abs(destinationY - pieceY)){
                if (i == pieceY) continue;
                if (board[i * 8 + pieceX] != EMPTY){
                    return false;
                }
            }
            return true;
        } else if (pieceX - destinationX == pieceY - destinationY){
            for (char i = pieceX; i != destinationX; i += (destinationX - pieceX) / SDL_abs(destinationX - pieceX)){
                if (i == pieceX) continue;
                if (board[(pieceY + (i - pieceX)) * 8 + i] != EMPTY){
                    return false;
                }
            }
            return true;
        } else if (SDL_abs(pieceX - destinationX) == SDL_abs(pieceY - destinationY)){
            for (char i = pieceX; i != destinationX; i += (destinationX - pieceX) / SDL_abs(destinationX - pieceX)){
                if (i == pieceX) continue;
                if (board[(pieceY - (i - pieceX)) * 8 + i] != EMPTY){
                    return false;
                }
            }
            return true;
        }

        return false;
    case WHITE_ROCK:
    case BLACK_ROCK:
        if (destinationY == pieceY){
            for (char i = pieceX; i != destinationX; i += (destinationX - pieceX) / SDL_abs(destinationX - pieceX)){
                if (i == pieceX) continue;
                if (board[pieceY * 8 + i] != EMPTY){
                    return false;
                }
            }
            return true;
        } else if (destinationX == pieceX){
            for (char i = pieceY; i != destinationY; i += (destinationY - pieceY) / SDL_abs(destinationY - pieceY)){
                if (i == pieceY) continue;
                if (board[i * 8 + pieceX] != EMPTY){
                    return false;
                }
            }
            return true;
        }
        return false;

        
    case WHITE_PAWN:

        if (indexPiece - 16 == indexDestination && pieceY == 6 && board[indexPiece - 8] == EMPTY && board[indexPiece - 16] == EMPTY){
            return true;
        }

        if (destinationY == 0){
            if (indexPiece - indexDestination == 8 && board[indexDestination] != EMPTY)
                return false;
            board[indexPiece] = EMPTY;
            PromatePawn(indexDestination);
            return false;
        }

        if ((indexPiece - indexDestination == 7 || indexPiece - indexDestination == 9) && board[indexDestination] != EMPTY){
            return true;
        }


        if (indexPiece - indexDestination != 8){
            return false;
        }

        if (board[indexPiece - 8] != EMPTY){
            return false;
        }

        

        return true;
    case BLACK_PAWN:
        
        if (indexPiece + 16 == indexDestination && pieceY == 1 && board[indexPiece + 8] == EMPTY && board[indexPiece + 16] == EMPTY){
            return true;
        }

        if (destinationY == 7){
            if (indexPiece - indexDestination == -8 && board[indexDestination] != EMPTY)
                return false;
            board[indexPiece] = EMPTY;
            PromatePawn(indexDestination);
            return false;
        }

        if ((indexPiece - indexDestination == -7 || indexPiece - indexDestination == -9) && board[indexDestination] != EMPTY){
            return true;
        }


        if (indexPiece - indexDestination != -8){
            return false;
        }

        if (board[indexPiece + 8] != EMPTY){
            return false;
        }

        return true;
    case WHITE_KING:
    case BLACK_KING:

        if (SDL_abs(pieceX - destinationX) <= 1 && SDL_abs(pieceY - destinationY) <= 1)
            return true;
        return false;
    
    default:
        return true;
    }
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

    SDL_Event event;

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
				case SDL_MOUSEBUTTONDOWN:
					
					if (selected != -1){
						char buffer = board[selected];

                        if (IsMoveLegal(selected, (mousePosX / squareWidth) + (mousePosY / squareHeight) * 8)){
                            board[selected] = EMPTY;
						    board[(mousePosX / squareWidth) + (mousePosY / squareHeight) * 8] = buffer;
                        }

						selected = -1;
						break;
					}
					selected = (mousePosX / squareWidth) + (mousePosY / squareHeight) * 8;

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
