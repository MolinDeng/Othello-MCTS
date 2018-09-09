#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "types.h"

unsigned long long TimeMilliseconds();
void InitMonteCarlo(monte_carlo *MCTS, board *Board, player *Player, int TimeInSeconds);
void UpdateState(monte_carlo *MCTS, board *Board);
position GetMove(monte_carlo *MCTS);
void RunSimulation(monte_carlo *MCTS);

#endif
