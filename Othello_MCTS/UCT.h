#pragma once
#include "TreeNode.h"
#include <cfloat>
#include <chrono>
#include<iomanip>
#include "Thread_guard.h"

class UCT {
private:
	int iterations;
public:
	float uct_k;
	int max_iterations;

	UCT() :
		uct_k(sqrt(2)), 
		max_iterations(10000000){}

	UCT(int iter):
		uct_k(sqrt(2)),
		max_iterations(iter){}

	TreeNode* get_best_uct_child(TreeNode* node) {
		if (!node->is_fully_expanded()) return NULL;

		float best_uct_score = -RAND_MAX;
		TreeNode* best_uct_node = NULL;

		int num_children = node->get_nChildren();
		for (int i = 0; i < num_children; i++) {
			TreeNode* child = node->get_child(i);
			float uct_exploitation = (float)child->get_totValues() / (child->get_nVisits() + FLT_EPSILON);
			float uct_exploration = sqrt(log((float)node->get_nVisits() + 1) / (child->get_nVisits() + FLT_EPSILON));
			float uct_score = uct_exploitation + uct_exploration;

			if (uct_score > best_uct_score) {
				best_uct_score = uct_score;
				best_uct_node = child;
			}
		}

		return best_uct_node;
	}

	TreeNode* get_most_visited_child(TreeNode* node) {
		int most_visits = -1;
		TreeNode* most_visited_node = NULL;

		// iterate all immediate children and find most visited
		int num_children = node->get_nChildren();
		for (int i = 0; i < num_children; i++) {
			TreeNode* child = node->get_child(i);
			if (child->get_nVisits() > most_visits) {
				most_visits = child->get_nVisits();
				most_visited_node = child;
			}
		}

		return most_visited_node;
	}

	TreeNode* get_most_winning_rate_child(TreeNode* node) {
		float best_rate = -1;
		TreeNode* best_child = NULL;
		int num_children = node->get_nChildren();
		for (int i = 0; i < num_children; i++) {
			TreeNode* child = node->get_child(i);
			if (child->get_totValues() / (float)child->get_nVisits() > best_rate) {
				best_rate = child->get_totValues() / (float)child->get_nVisits();
				best_child = child;
			}
		}
		return best_child;
	}

	position run(player _player, player _opponent, board _board) {
		TreeNode _root(_player, _opponent, _board);
		TreeNode* best_node = NULL;
		_time start_time = getmilliseconds();
		iterations = 0;
		while (true) {
			//Selection
			TreeNode* ptr_node = &_root;
			while (!ptr_node->is_terminated() && ptr_node->is_fully_expanded())
				ptr_node = get_best_uct_child(ptr_node);
				
			//Expansion
			if (!ptr_node->is_fully_expanded() && !ptr_node->is_terminated())
				ptr_node = ptr_node->expand();
				

			//Simulation
			State state(ptr_node->get_state());
			if (!state.is_terminated()) {
				//模拟至游戏结束
				othello Othello;
				create_tmp_game(Othello, state);
				piece_color PlayerTurn = state.Player.Color;
				while (Othello.Board.EmptyPieces != 0) {
					if (Othello.LightPlayer.OutOfMoves && Othello.DarkPlayer.OutOfMoves)
						break;
					player *ActivePlayer = FindActivePlayer(&Othello, PlayerTurn);
					std::vector<position> Moves = GetLegalMoves(&Othello.Board, ActivePlayer);
					if (!Moves.empty()) {
						int Choice;
						ActivePlayer->OutOfMoves = false;
						PerformMove(&Othello.Board, &Moves[Search(Moves.size())], ActivePlayer);
					}
					else ActivePlayer->OutOfMoves = true;
					PlayerTurn = PlayerTurn == COLOR_LIGHT ? COLOR_DARK : COLOR_LIGHT;
				}
				state.Board = Othello.Board;
			}

			//Back propagation
			//ptr_node->backprop(state.evaluate());
			float delta = state.evaluate();
			while (ptr_node) {
				ptr_node->update(delta);
				ptr_node = ptr_node->get_parent();
			}

			//which one is better??
			best_node = get_most_winning_rate_child(&_root);
			//best_node = get_most_visited_child(&_root);
			//best_node = get_best_uct_child(&_root);

			if (getmilliseconds() - start_time > TIME_LIMITATION
				|| iterations > max_iterations) break;
			iterations++;
		}
		/*for(auto node : _root.children)
			std::cout << node->get_totValues() << "/" << node->get_nVisits() << std::endl;
		std::cout << best_node->get_totValues() << "/" << best_node->get_nVisits() << std::endl;
		*/
		std::cout << "Winning rate: " << std::setprecision(4)
			<< 100 * _root.get_totValues() / (float)_root.get_nVisits() 
			<< "%" << std::endl;
		std::cout << "iterations: " << iterations << std::endl;
		position ret = best_node->state._p;
		_root.clean();
		return ret;
	}

	void create_tmp_game(othello& Othello, State& state) {
		Othello.Board = state.Board;
		if (state.Player.Color == COLOR_DARK) {
			Othello.DarkPlayer = state.Player;
			Othello.LightPlayer = state.Opponent;
		}
		else {
			Othello.DarkPlayer = state.Opponent;
			Othello.LightPlayer = state.Player;
		}
	}
};