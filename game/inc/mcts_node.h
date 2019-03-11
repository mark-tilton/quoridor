#ifndef QUORIDORBOT_MCTS_NODE_H_
#define QUORIDORBOT_MCTS_NODE_H_

#include "player.h"
#include <vector>
#include <optional>

class MctsNode {

public:
	// Constructors
	MctsNode(const BoardState board_state, const std::optional<Action> action, MctsNode* parent);

	// Accessors
	MctsNode& operator[] (int i);
	std::vector<MctsNode>::iterator begin();
	std::vector<MctsNode>::iterator end();
	const BoardState& GetBoardState() const;
	const std::optional<Action>& GetAction() const;
	MctsNode* GetParent() const;
	int GetChildCount() const;
	int GetVisitCount() const;
	double GetScore() const;

	// Mutators
	std::vector<MctsNode> children_;
	void Expand(MctsNode child);
	void Visit(double score);


private:
	const std::optional<Action> action_;
	double score_ = 0;
	int visit_count_ = 0;
	MctsNode* parent_;
	const BoardState board_state_;

};

#endif