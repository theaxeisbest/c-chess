#include <stdbool.h>

bool IsMoveLegal(char indexPiece, char indexDestination, char* board);
bool CanCapturePiece(char indexKing);
char* AllLegealMoves(char* board);
void AnPassant(char pawnIndex);
void CastleKing(char kingIndex);
void PromatePawn(char pawnIndex, char* board);
void BoardAfterMove(char* board, char indexPiece, char indexDestination);
unsigned short int NumberOfMoves(char* moveList);
char* CopyBoard(char* board);
char* filterColor(char* moves, char* board, bool filterBlack);