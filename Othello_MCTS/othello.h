#pragma once
#include "types.h"
#include "easyx.h"
#include "graphics.h"
#include <strstream>
#include <ctime>

othello CreateNewGame_AIfist();
othello CreateNewGame_Humanfist();
board CreateNewBoard();
player CreateNewPlayer(std::string Name, piece_color Color, bool AI);
player *FindActivePlayer(othello *Othello, piece_color PlayerTurn);
void CountPiecesOfPlayer(board *Board, player *Player);
player *FindWinner(othello *Othello);
std::vector<position> GetLegalMoves(board *Board, player *Player);
bool IsMoveLegal(board *Board, position *Pos, player *Player);
void PerformMove(board *Board, position *Pos, player *Player);
void PrintBoard(board *Board);
void SetWindows();
int Search(int size);
int HumanChoice(std::vector<position> &Moves);
void MarkLastMove(position *Pos, piece_color Color);
_time getmilliseconds();
//bool GamePlay();
bool UCT_GamePlay();