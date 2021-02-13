#include "squares.h"
#include "engine.h"
#include "moves.h"
#include <stdlib.h>
#include <time.h>

unsigned int EvaluateBoard(char* board){
    int ret = 0;
    for (char i = 0; i < 64; i++){
        switch (board[i])
        {
        case EMPTY:
            break;
        case WHITE_PAWN:
            ret += 1000;
            break;
        case BLACK_PAWN:
            ret -= 1000;
            break;
        case WHITE_BISHOP:
            ret += 3050;
            break;
        case BLACK_BISHOP:
            ret -= 3050;
            break;
        case WHITE_KNIGHT:
            ret += 3000;
            break;
        case BLACK_KNIGHT:
            ret -= 3000;
            break;
        case WHITE_ROCK:
            ret += 5000;
            break;
        case BLACK_ROCK:
            ret -= 5000;
            break;
        case WHITE_QUEEN:
            ret += 9000;
            break;
        case BLACK_QUEEN:
            ret -= 9000;
            break;
        case WHITE_KING:
            ret += 100000000;
            break;
        case BLACK_KING:
            ret -= 100000000;
            break;
        default:
            break;
        }
    }
    return ret;
}

int* GenerateTree(unsigned int depth, char* startBoard){
    

    char checkBoard[64];

    unsigned short int* branches = malloc(2 * depth);

}

char* MakeBestMove(char* board){
    char* moves = AllLegealMoves(board);
    char* ret = malloc(2);

    srand(time(NULL));

    short unsigned int move;

    do {
        move = rand() % NumberOfMoves(moves);
    } while (board[moves[move * 2]] > EMPTY);

    ret[0] = moves[move * 2];
    ret[1] = moves[(move * 2) + 1];

    free(moves);

    return ret; 
}