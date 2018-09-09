#pragma once
#include "State.h"

class TreeNode {
private:
	State state;//棋局与玩家
	int nVisits;//访问次数
	float totValues;//获利

	TreeNode* parent;
	std::vector<TreeNode*> children;

	friend class UCT;

	void clean() {
		state.clean();
		children.swap(std::vector<TreeNode*>());
	}

public:
	TreeNode(player pl, player op, board b, TreeNode* p = NULL) :
		state(pl, op, b), nVisits(0), totValues(0), parent(p) {
	}

	bool is_terminated() { return state.is_terminated(); }

	State get_state()const { return state; }

	bool is_fully_expanded() const { return !children.empty() && children.size() == state.Moves.size(); }

	int get_nVisits() const { return nVisits; }

	float get_totValues() const { return totValues; }

	int get_nChildren() const { return children.size(); }

	TreeNode* get_child(int i) const { return children[i]; }

	TreeNode* get_parent() const { return parent; }

	TreeNode* expand() {
		if (is_fully_expanded())	return NULL;

		board tmp_board = state.Board;
		position p = state.apply_random_action(&tmp_board);
		TreeNode* child_node = new TreeNode(state.Opponent, state.Player, tmp_board, this);
		child_node->state._p = p;
		children.push_back(child_node);
		return child_node;
	}
	void backprop(float delta) {
		TreeNode* tmp = this;
		while (tmp) {
			tmp->nVisits++;
			tmp->totValues += delta;
			tmp = tmp->parent;
		}
	}
	void update(float delta) {
		totValues += delta;
		nVisits++;
	}
};