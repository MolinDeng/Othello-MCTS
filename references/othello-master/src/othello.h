#ifndef OTHELLO_H
#define OTHELLO_H

#include "types.h"

othello CreateNewGame();
board CreateNewBoard();
player CreateNewPlayer(std::string Name, piece_color Color, bool AI);
player *FindActivePlayer(othello *Othello, piece_color PlayerTurn);
void CountPiecesOfPlayer(board *Board, player *Player);
player *FindWinner(othello *Othello);
std::vector<position> GetLegalMoves(board *Board, player *Player);
bool IsMoveLegal(board *Board, position *Pos, player *Player);
void PerformMove(board *Board, position *Pos, player *Player);
void PrintBoard(board *Board);

#endif

