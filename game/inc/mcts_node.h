#ifndef QUORIDORBOT_MCTS_NODE_H_
#define QUORIDORBOT_MCTS_NODE_H_

#include "player.h"
#include <vector>

class MctsNode {

public:
	// Constructors
	MctsNode(const BoardState& board_state);
	MctsNode(MctsNode* parent, const Action& action, int player_index);

	// Accessors
	MctsNode& operator[] (int i);
	MctsNode* begin();
	MctsNode* end();
	const BoardState& GetBoardState() const;
	MctsNode* GetParent() const;
	const Action& GetAction() const;
	int GetChildCount() const;
	int GetVisitCount() const;
	double GetScore() const;

	// Mutators
	void Expand(const std::vector<Action> actions, const int player_index);
	void Visit(double score);


private:
	const Action action_;
	double score_ = 0;
	int visit_count_ = 0;
	MctsNode* parent_;
	const BoardState board_state_;
	std::vector<MctsNode> children_;

};

std::ostream& operator<< (std::ostream& os, const MctsNode& n);

#endif