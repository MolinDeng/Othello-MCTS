#include "montecarlo.h"
#include "othello.h"
#include <chrono>

unsigned long long TimeMilliseconds()
{
   using namespace std::chrono;
   return (duration_cast<milliseconds>(steady_clock::now().time_since_epoch())).count();
}

void InitMonteCarlo(monte_carlo *MCTS, board *Board, player *Player, int TimeInSeconds)
{
    MCTS->Player = *Player;
    MCTS->Board = *Board;

    MCTS->TimeInSeconds = TimeInSeconds;
    MCTS->MaxNumberOfMoves = 100;
}

void UpdateState(monte_carlo *MCTS, board *Board)
{
    MCTS->Board = *Board;
}

position GetMove(monte_carlo *MCTS)
{
    position Move = {};

    int StartTime = TimeMilliseconds();
    while(TimeMilliseconds() - StartTime < MCTS->TimeInSeconds * 1000)
        RunSimulation(MCTS);

    return Move;
}

void RunSimulation(monte_carlo *MCTS)
{
    board Board = MCTS->Board;

    for(int Move = 0; Move < MCTS->MaxNumberOfMoves; ++Move)
    {
        std::vector<position> LegalMoves = GetLegalMoves(&Board, MCTS->Player);

        Position *Play = Choice(LegalMoves);
        PerformMove(&Board, Play, &MCTS->Player);
    }
}
