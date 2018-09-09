#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>


#define BOARD_SIZE 8

enum piece_color
{
    COLOR_LIGHT,
    COLOR_DARK,
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

struct monte_carlo
{
    player Player;

    board Board;
    int MaxDepth;
    int TimeInSeconds;
    int MaxNumberOfMoves;

    std::vector<int> Wins;
    std::vector<int> Plays;
};

struct othello
{
    board Board;
    player LightPlayer;
    player DarkPlayer;
};

#endif
