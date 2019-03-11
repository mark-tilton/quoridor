#include "mcts_node.h"
#include <utility>

using namespace std;

MctsNode::MctsNode(const BoardState& board_state, std::optional<Action> action, MctsNode* parent) :
	action_(std::move(action)),
	parent_(parent),
	board_state_(board_state) {

}

MctsNode& MctsNode::operator[](int i)
{
	return children_[i];
}

vector<MctsNode>::iterator MctsNode::begin()
{
	return children_.begin();
}

vector<MctsNode>::iterator MctsNode::end()
{
	return children_.end();
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

void MctsNode::Expand(const MctsNode& child)
{
	children_.push_back(child);
}

void MctsNode::Visit(double score) {
	visit_count_++;
	score_ += score;
}