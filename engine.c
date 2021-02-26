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

char* MakeBestMoveRandom(char* board){
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

unsigned int RetMin(unsigned int* list, unsigned short int size){
    unsigned short int ret = 0;
    for (unsigned short int n = 1; n < size; n++){
        if (list[ret] > list[n]){
            ret = n;
        }
    }
}

unsigned int RetMax(unsigned int* list, unsigned short int size){
    unsigned short int ret = 0;
    for (unsigned short int n = 1; n < size; n++){
        if (list[ret] < list[n]){
            ret = n;
        }
    }
}

unsigned int MiniMaxBoard(char* board, short unsigned int deapth, bool blacksTurn){

    if (deapth == 0){
        return EvaluateBoard(board);
    }

    char* allMoves = AllLegealMoves(board);

    const unsigned short int allMovesLen = NumberOfMoves(allMoves);

    unsigned int* moveValues = malloc(allMovesLen); 

    for (unsigned short int n = 0; n < allMovesLen; n += 1){

        char* copyOfBoard = CopyBoard(board);

        BoardAfterMove(copyOfBoard, n*2, (n*2)+1);

        moveValues[n] /* <-- same as / 2 (just a smudge bit faster)*/ = MiniMaxBoard(board, deapth - 1, !blacksTurn);

        free(copyOfBoard);
    }

    unsigned int indexRet;

    if (blacksTurn){
        indexRet = RetMin(moveValues, allMovesLen);
    } else {
        indexRet = RetMax(moveValues, allMovesLen);
    }

    unsigned int ret = moveValues[indexRet];

    free(moveValues);

    return ret;
}

char* MakeBestMove(char* board, short unsigned int deapth){
    char* allMoves = AllLegealMoves(board);

    allMoves = filterColor(allMoves, board, true);

    const unsigned short int allMovesLen = NumberOfMoves(allMoves);

    unsigned int* moveValues = malloc(allMovesLen*4); 
     
    for (unsigned short int n = 0; n < allMovesLen; n += 1){

        char* copyOfBoard = CopyBoard(board);

        BoardAfterMove(copyOfBoard, allMoves[n*2], allMoves[(n*2)+1]);
 
        moveValues[n] = EvaluateBoard(copyOfBoard);

        //free(copyOfBoard);
    }

    unsigned int indexRet = RetMin(moveValues, allMovesLen);

    //free(moveValues);

    char* ret = malloc(2);

    ret[0] = allMoves[indexRet * 2];
    ret[1] = allMoves[(indexRet * 2) + 1];

    return ret;
}

/*
char* MakeBestMove(char* board, short unsigned int deapth){
    char* allMoves = AllLegealMoves(board);

    allMoves = filterColor(allMoves, board, true);

    const unsigned short int allMovesLen = NumberOfMoves(allMoves);

    char* retVal = malloc(allMovesLen * 2);

    unsigned int* moveValues = malloc(allMovesLen) 

    for (unsigned short int i = 0; i < allMovesLen; i++){
        char* copyOfBoard = CopyBoard(board);

        BoardAfterMove(copyOfBoard, allMoves[i * 2], allMoves[(i * 2) + 1]);

         = EvaluateBoard(copyOfBoard);



        free(copyOfBoard);
    }
}
*/