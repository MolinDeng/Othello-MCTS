#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <streambuf>
#define BOARD_SIZE 8
#define TIME_LIMITATION 20000
typedef unsigned long long _time;

enum piece_color
{
	COLOR_DARK,
	COLOR_LIGHT,
	COLOR_EMPTY
};

struct position
{
	int Row;
	int Col;

	std::vector<position> Captured;
};

struct board
{
	piece_color State[BOARD_SIZE][BOARD_SIZE];
	int EmptyPieces;
};

struct player
{
	std::string Name;
	piece_color Color;
	bool OutOfMoves;
	int Pieces;

	bool AIPlayer;
};

struct othello
{
	board Board;
	player LightPlayer;
	player DarkPlayer;
};
