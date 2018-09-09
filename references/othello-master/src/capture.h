#ifndef CAPTURE_H
#define CAPTURE_H

#include "types.h"

bool CheckNorth(board *Board, position *Pos, player *Player);
bool CheckSouth(board *Board, position *Pos, player *Player);
bool CheckEast(board *Board, position *Pos, player *Player);
bool CheckWest(board *Board, position *Pos, player *Player);
bool CheckNorthWest(board *Board, position *Pos, player *Player);
bool CheckNorthEast(board *Board, position *Pos, player *Player);
bool CheckSouthWest(board *Board, position *Pos, player *Player);
bool CheckSouthEast(board *Board, position *Pos, player *Player);

#endif
