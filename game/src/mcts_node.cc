#include "mcts_node.h"

using namespace std;

MctsNode::MctsNode(const BoardState board_state, const std::optional<Action> action, MctsNode* parent) :
	board_state_(board_state),
	parent_(parent),
	action_(action),
	children_(vector<MctsNode>()),
	visit_count_(0),
	score_(0) {

}

MctsNode& MctsNode::operator[](int i)
{
	return children_[i];
}

MctsNode* MctsNode::begin()
{
	return &children_[0];
}

MctsNode* MctsNode::end()
{
	return &children_[children_.size() - 1];
}

const BoardState& MctsNode::GetBoardState() const
{
	return board_state_;
}

const std::optional<Action>& MctsNode::GetAction() const
{
	return action_;
}

MctsNode* MctsNode::GetParent() const
{
	return parent_;
}

int MctsNode::GetChildCount() const
{
	return children_.size();
}

int MctsNode::GetVisitCount() const {
	return visit_count_;
}

double MctsNode::GetScore() const {
	return score_;
}

void MctsNode::Expand(MctsNode child)
{
	children_.push_back(child);
}

void MctsNode::Visit(double score) {
	visit_count_++;
	score_ += score;
}