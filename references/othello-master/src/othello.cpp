#include "othello.h"
#include "capture.h"
#include <time.h>

othello CreateNewGame()
{
    othello Othello = {};

    Othello.Board = CreateNewBoard();
    Othello.LightPlayer = CreateNewPlayer("Human", COLOR_LIGHT, false);
    Othello.DarkPlayer = CreateNewPlayer("AI", COLOR_DARK, true);

    return Othello;
}

board CreateNewBoard()
{
    board Board = {};
    for(int col = 0; col < BOARD_SIZE; ++col)
    {
        for(int row = 0; row < BOARD_SIZE; ++row)
            Board.State[row][col] = COLOR_EMPTY;
    }

    Board.State[3][3] = COLOR_LIGHT;
    Board.State[3][4] = COLOR_DARK;
    Board.State[4][3] = COLOR_DARK;
    Board.State[4][4] = COLOR_LIGHT;
    Board.EmptyPieces = 60;

    return Board;
}

player CreateNewPlayer(std::string Name, piece_color Color, bool AI)
{
    player Player = {};

    Player.Name = Name;
    Player.Color = Color;
    Player.Pieces = 0;
    Player.OutOfMoves = false;
    Player.AIPlayer = AI;

    return Player;
}

player *FindActivePlayer(othello *Othello, piece_color PlayerTurn)
{
    return PlayerTurn == COLOR_DARK ? &Othello->DarkPlayer : &Othello->LightPlayer;
}

void CountPiecesOfPlayer(board *Board, player *Player)
{
    Player->Pieces = 0;
    for(int Col = 0; Col < BOARD_SIZE; ++Col)
    {
        for(int Row = 0; Row < BOARD_SIZE; ++Row)
        {
            if(Board->State[Row][Col] == Player->Color)
                ++Player->Pieces;
        }
    }
}

player *FindWinner(othello *Othello)
{
    if(Othello->LightPlayer.Pieces > Othello->DarkPlayer.Pieces)
        return &Othello->LightPlayer;
    else if(Othello->LightPlayer.Pieces < Othello->DarkPlayer.Pieces)
        return &Othello->DarkPlayer;
    else
        return NULL;

}

std::vector<position> GetLegalMoves(board *Board, player *Player)
{
    std::vector<position> Moves;
    for(int Col = 0; Col < BOARD_SIZE; ++Col)
    {
        for(int Row = 0; Row < BOARD_SIZE; ++Row)
        {
            position Pos = { Row, Col };
            if(IsMoveLegal(Board, &Pos, Player))
                Moves.push_back(Pos);
        }
    }

    return Moves;
}

bool IsMoveLegal(board *Board, position *Pos, player *Player)
{
    if((Pos->Row >= BOARD_SIZE || Pos->Row < 0) ||
       (Pos->Col >= BOARD_SIZE || Pos->Col < 0) ||
       (Board->State[Pos->Row][Pos->Col] != COLOR_EMPTY))
        return false;

    bool Result = false;

    if(CheckNorth(Board, Pos, Player))
        Result = true;
    if(CheckEast(Board, Pos, Player))
        Result = true;
    if(CheckSouth(Board, Pos, Player))
        Result = true;
    if(CheckWest(Board, Pos, Player))
        Result = true;
    if(CheckNorthEast(Board, Pos, Player))
        Result = true;
    if(CheckNorthWest(Board, Pos, Player))
        Result = true;
    if(CheckSouthEast(Board, Pos, Player))
        Result = true;
    if(CheckSouthWest(Board, Pos, Player))
        Result = true;

    return Result;
}

void PerformMove(board *Board, position *Pos, player *Player)
{
    Board->State[Pos->Row][Pos->Col] = Player->Color;
    for(int Index = 0; Index < Pos->Captured.size(); ++Index)
    {
        position Piece = Pos->Captured[Index];
        Board->State[Piece.Row][Piece.Col] = Player->Color;
    }

    Pos->Captured.clear();
    --Board->EmptyPieces;
}

void PrintBoard(board *Board)
{
    printf("\n");
    printf("  ");
    for(char Col = 'a'; Col <= 'h'; ++Col)
        printf("%c ", Col);

    printf("\n");
    for(int Row = 0; Row < BOARD_SIZE; ++Row)
    {
        printf("%d ", Row+1);
        for(int Col = 0; Col < BOARD_SIZE; ++Col)
        {
            char Output = ' ';
            if(Board->State[Row][Col] == COLOR_LIGHT)
                Output = 'L';
            else if(Board->State[Row][Col] == COLOR_DARK)
                Output = 'D';

            printf("%c ", Output);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    othello Othello = CreateNewGame();
    srand(time(NULL));

    piece_color PlayerTurn = COLOR_DARK;
    while(Othello.Board.EmptyPieces != 0)
    {
        if(Othello.LightPlayer.OutOfMoves && Othello.DarkPlayer.OutOfMoves)
            break;

        PrintBoard(&Othello.Board);

        player *ActivePlayer = FindActivePlayer(&Othello, PlayerTurn);
        std::vector<position> Moves = GetLegalMoves(&Othello.Board, ActivePlayer);
        if(!Moves.empty())
        {
            ActivePlayer->OutOfMoves = false;
            printf("%s, Legal Moves\n", PlayerTurn == COLOR_LIGHT ? "Light" : "Dark");
            for(int Move = 0; Move < Moves.size(); ++Move)
                printf("%d:    Move - (%d,%c)\n", Move, Moves[Move].Row+1, Moves[Move].Col+'a');

            if(ActivePlayer->AIPlayer)
            {
                /* TODO: Replace with generated move */
                int Choice = rand() % Moves.size();
                PerformMove(&Othello.Board, &Moves[Choice], ActivePlayer);
                sleep(1);
                printf("Selected move: %d\n", Choice);
            }
            else
            {
                int Choice = -1;
                do{
                    printf("Select a move: ");
                    std::cin >> Choice;
                } while(Choice < 0 || Choice >= Moves.size());
                PerformMove(&Othello.Board, &Moves[Choice], ActivePlayer);
            }
        }
        else
        {
            printf("Player %s are out of legal moves\n", ActivePlayer->Name.c_str());
            ActivePlayer->OutOfMoves = true;
        }

        PlayerTurn = PlayerTurn == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;
    }

    PrintBoard(&Othello.Board);

    CountPiecesOfPlayer(&Othello.Board, &Othello.LightPlayer);
    CountPiecesOfPlayer(&Othello.Board, &Othello.DarkPlayer);
    player *Winner = FindWinner(&Othello);
    if(!Winner)
        std::cout << "Draw!" << std::endl;
    else
        std::cout << Winner->Name << " won with " << std::to_string(Winner->Pieces) << " pieces!" << std::endl;
}
