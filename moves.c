#include "squares.h"
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

float BoardDistance(char indexPoint1, char indexPoint2){
	char pieceX = indexPoint1 % 8;
	char pieceY = indexPoint1 / 8;

	char destinationX = indexPoint2 % 8;
	char destinationY = indexPoint2 / 8;

	return sqrt((pieceX - destinationX) * (pieceX - destinationX) + (pieceY - destinationY) * (pieceY - destinationY));
}

void AddMoveToList(char* list, Uint16* destination, char indexPiece, char indexDestination){

	if ((unsigned char)(indexDestination) >= 64){
		return;
	}

	list[*destination] = indexPiece;
	list[*destination + 1] = indexDestination;

	*destination += 2;
}

char* AllLegealMoves(char* board){

	char* moveList = malloc(1024 * 2);
	Uint16 listPointer = 0;

	for (int i = 0; i < 1024 * 2; i++){
		moveList[i] = 0;
	}

	for (char indexPiece = 0; indexPiece < 64; indexPiece++){

		const signed char pieceX = indexPiece % 8;
		const signed char pieceY = indexPiece / 8;

		const bool isBlack = board[indexPiece] < EMPTY;

		const signed char LegealMovesKnight[] = {-10,6,15,17,10,-6,-15,-17};
		const signed char LegealMovesBishop[] = {7, 9, -7, -9};
		unsigned char xPointer = pieceX; 
		signed char PointerAdd = -1;
		unsigned char yPointer = pieceY;

		switch (board[indexPiece])
		{
		case EMPTY:
			//return false;
			break;
		case WHITE_BISHOP:
		case BLACK_BISHOP:;

			

			

			for (char i = 0; i < 4; i++){


				const signed char activeDirection = LegealMovesBishop[i];
			
				unsigned char destination = indexPiece + activeDirection;

				if ((unsigned char)(destination) >= 64 ||
						abs(pieceX - (destination) % 8) != abs(pieceY - (destination) / 8)){
						goto bishopWhileEnd;
					}

				while (board[destination] == EMPTY){
					AddMoveToList(moveList, &listPointer, indexPiece, destination);

					if ((unsigned char)(destination + activeDirection) >= 64 ||
						abs(pieceX - (destination + activeDirection) % 8) != abs(pieceY - (destination + activeDirection) / 8)){
						goto bishopWhileEnd;
					}

					destination += activeDirection;
				}

				if (isBlack){			 
					if (board[destination] > EMPTY) { 
						AddMoveToList(moveList, &listPointer, indexPiece, destination); 
					}
				}
				else{
					if (board[destination] < EMPTY) { 
						AddMoveToList(moveList, &listPointer, indexPiece, destination); 
					}
				}	
				bishopWhileEnd:;
			}

			break;
			
		case WHITE_KNIGHT:
		case BLACK_KNIGHT:
			;
			

			for (char i = 0; i < 8; i++){
				if (board[indexPiece] > EMPTY){
					if (board[(indexPiece + LegealMovesKnight[i])] <= EMPTY){
						
						

						if (BoardDistance(indexPiece, indexPiece + LegealMovesKnight[i]) == 2.236068f ||
						 BoardDistance(indexPiece, indexPiece + LegealMovesKnight[i]) == 6.324555f){
							AddMoveToList(moveList, &listPointer, indexPiece, indexPiece + LegealMovesKnight[i]);
						}

					}
				} else {
					if (board[(indexPiece + LegealMovesKnight[i])] >= EMPTY){
						if (BoardDistance(indexPiece, indexPiece + LegealMovesKnight[i]) == 2.236068f ||
						 BoardDistance(indexPiece, indexPiece + LegealMovesKnight[i]) == 6.324555f){
							AddMoveToList(moveList, &listPointer, indexPiece, indexPiece + LegealMovesKnight[i]);
						}
					}
				}
			}

			break;

			//return false;
		case WHITE_QUEEN:
		case BLACK_QUEEN:;

			for (char i = 0; i < 4; i++){


				const signed char activeDirection = LegealMovesBishop[i];
			
				unsigned char destination = indexPiece + activeDirection;

				if ((unsigned char)(destination) >= 64 ||
						abs(pieceX - (destination) % 8) != abs(pieceY - (destination) / 8)){
						goto queenWhileEnd;
					}

				while (board[destination] == EMPTY){
					AddMoveToList(moveList, &listPointer, indexPiece, destination);

					if ((unsigned char)(destination + activeDirection) >= 64 ||
						abs(pieceX - (destination + activeDirection) % 8) != abs(pieceY - (destination + activeDirection) / 8)){
						goto queenWhileEnd;
					}

					destination += activeDirection;
				}

				if (isBlack){			 
					if (board[destination] > EMPTY) { 
						AddMoveToList(moveList, &listPointer, indexPiece, destination); 
					}
				}
				else{
					if (board[destination] < EMPTY) { 
						AddMoveToList(moveList, &listPointer, indexPiece, destination); 
					}
				}	
				queenWhileEnd:;
			}

			

			startXQueen:;
			
			xPointer = pieceX; 

			while(xPointer < 8){ //x movment

				xPointer += PointerAdd;

				if (isBlack ? board[pieceY * 8 + xPointer] > EMPTY : board[pieceY * 8 + xPointer] < EMPTY){
					AddMoveToList(moveList, &listPointer, indexPiece, pieceY * 8 + xPointer);
					break;
				}
				if (isBlack ? board[pieceY * 8 + xPointer] < EMPTY : board[pieceY * 8 + xPointer] > EMPTY){ break; }

				AddMoveToList(moveList, &listPointer, indexPiece, pieceY * 8 + xPointer);

			}

			if (PointerAdd == -1) {PointerAdd = 1; goto startXQueen;}

			PointerAdd = -1;

			startYQueen:;

			yPointer = pieceY;

		

			while (yPointer < 8){
				yPointer += PointerAdd;

				if (isBlack ? board[yPointer * 8 + pieceX] > EMPTY : board[yPointer * 8 + pieceX] < EMPTY){
					AddMoveToList(moveList, &listPointer, indexPiece, yPointer * 8 + pieceX);
					break;
				}
				if (isBlack ? board[yPointer * 8 + pieceX] < EMPTY : board[yPointer * 8 + pieceX] > EMPTY){ break; }

				AddMoveToList(moveList, &listPointer, indexPiece, yPointer * 8 + pieceX);

			}

			if (PointerAdd == -1) {PointerAdd = 1; goto startYQueen;}

			PointerAdd = -1;

			break;

		case BLACK_ROCK:;
		case WHITE_ROCK:;

//			signed char PointerAdd = -1;

			startX:;
			
			xPointer = pieceX; 

			while(xPointer < 8){ //x movment

				if ((unsigned char)(xPointer + PointerAdd) >= 8){
					break;
				}

				xPointer += PointerAdd;

				

				if (isBlack ? board[pieceY * 8 + xPointer] > EMPTY : board[pieceY * 8 + xPointer] < EMPTY){
					AddMoveToList(moveList, &listPointer, indexPiece, pieceY * 8 + xPointer);
					break;
				}
				if (isBlack ? board[pieceY * 8 + xPointer] < EMPTY : board[pieceY * 8 + xPointer] > EMPTY){ break; }

				AddMoveToList(moveList, &listPointer, indexPiece, pieceY * 8 + xPointer);

			}

			if (PointerAdd == -1) {PointerAdd = 1; goto startX;}

			PointerAdd = -1;

			startY:;

			yPointer = pieceY;

		

			while (yPointer < 8){

				if ((unsigned char)(yPointer + PointerAdd) >= 8){
					break;
				}

				yPointer += PointerAdd;

				if (isBlack ? board[yPointer * 8 + pieceX] > EMPTY : board[yPointer * 8 + pieceX] < EMPTY){
					AddMoveToList(moveList, &listPointer, indexPiece, yPointer * 8 + pieceX);
					break;
				}
				if (isBlack ? board[yPointer * 8 + pieceX] < EMPTY : board[yPointer * 8 + pieceX] > EMPTY){ break; }

				AddMoveToList(moveList, &listPointer, indexPiece, yPointer * 8 + pieceX);

			}

			if (PointerAdd == -1) {PointerAdd = 1; goto startY;}

			PointerAdd = -1;

			break;
			
		case WHITE_PAWN:

			if (board[indexPiece - 8] == EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece - 8);
			}

			if (pieceY == 6 && board[indexPiece - 16] == EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece - 16);
			}

			if ((indexPiece / 8) < pieceY - 1){
				break;
			}

			if (board[indexPiece - 7] < EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece - 7);
			}

			if (board[indexPiece - 9] < EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece - 9);
			}

			break;
		case BLACK_PAWN:
			if (pieceY == 1 && board[indexPiece + 16] == EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece + 16);
			}
			
			if (board[indexPiece + 8] == EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece + 8);
			}

			if ((indexPiece / 8) > pieceY + 1){
				break;
			}

			if (board[indexPiece + 7] > EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece + 7);
			}

			if (board[indexPiece + 9] > EMPTY){
				AddMoveToList(moveList, &listPointer, indexPiece, indexPiece + 9);
			}


			break;
			
		case WHITE_KING:
		case BLACK_KING:

			for (char i = -1; i < 2; i++){
				for (char j = -1; j < 2; j++){
					if (i == 0 && j == 0) { continue; }
					if (!(isBlack ? board[(pieceY + i) * 8 + (pieceX + j)] < EMPTY : board[(pieceY + i) * 8 + (pieceX + j)] > EMPTY)){
						AddMoveToList(moveList, &listPointer, indexPiece, (pieceY + i) * 8 + (pieceX + j));
					}
				}
			}
		
		default:
			break;
			//return true;
		}
	}
	return moveList;
}

void BoardAfterMove(char* board, char indexPiece, char indexDestination){

	if (board[indexPiece] == WHITE_PAWN || board[indexPiece] == BLACK_PAWN){
		const signed char pieceY = indexDestination / 8;
		if (pieceY == 0 || pieceY == 7){
			const bool isBlack = board[indexPiece] < EMPTY;
			board[indexDestination] = isBlack ? BLACK_QUEEN : WHITE_QUEEN;
			goto end;
		}
	}
	
	board[indexDestination] = board[indexPiece];

	end:;

	board[indexPiece] = EMPTY;
}

bool IsMoveLegal(char indexPiece, char indexDestination, char* board){
	char* allMoves = AllLegealMoves(board);

	for (int i = 0; i < 1024 * 2; i += 2){

		

		const char check1 = allMoves[i];
		const char check2 = allMoves[i + 1];

		if (check1 == indexPiece && check2 == indexDestination){
			free(allMoves);
			return true;
		}
	}
	free(allMoves);
	return false;
}

unsigned short int NumberOfMoves(char* moveList){
	for (Uint16 n = 0; n < 1024; n++){
		const Uint16* moveListUint16 = (Uint16*)moveList;

		if (moveListUint16[n] == 0){
			return n - 1;
		}

	}
}

char* CopyBoard(char* board){
	char* newBoard = malloc(64);

	for (char i = 0; i < 64; i++){
		newBoard[i] = board[i];
	}

	return newBoard;
}

char* filterColor(char* moves, char* board, bool filterBlack){

	char* newMoves = malloc(1024 * 2);
	unsigned int newMovesPointer = 0;

	if (filterBlack){
		for (unsigned int x = 0; x < 1024; x++){
			if (board[moves[x * 2]] < EMPTY){
				newMoves[newMovesPointer] = moves[x*2];
				newMoves[newMovesPointer + 1] = moves[x*2+1];
				newMovesPointer += 2;
			}
		}
			
	}else {
		for (unsigned int x = 0; x < 1024; x++){
			if (board[moves[x * 2]] > EMPTY){
				newMoves[newMovesPointer] = moves[x*2];
				newMoves[newMovesPointer + 1] = moves[x*2+1];
				newMovesPointer += 2;
			}
		}
	}

	free(moves);

	return newMoves;

}