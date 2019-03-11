#include "mcts_node.h"

using namespace std;

MctsNode::MctsNode(const BoardState& board_state) : 
	parent_(nullptr),
	board_state_(board_state),
	visit_count_(0),
	score_(0),
	action_(Action(Vectori(0, 0))),
	children_(vector<MctsNode>()) {

}

MctsNode::MctsNode(MctsNode* parent, const Action& action, int player_index) :
	board_state_(BoardState(parent->board_state_, action, player_index)),
	parent_(parent),
	action_(action),
	visit_count_(0),
	score_(0),
	children_(vector<MctsNode>()) {
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

MctsNode* MctsNode::GetParent() const
{
	return parent_;
}

const Action& MctsNode::GetAction() const
{
	return action_;
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

void MctsNode::Expand(const vector<Action> actions, const int player_index)
{
	for (auto& action : actions) {
		children_.emplace_back(this, action, player_index);
	}
}

void MctsNode::Visit(double score) {
	visit_count_++;
	score_ += score;
}

std::ostream& operator<< (std::ostream& os, const MctsNode& n) {
	os << n.GetAction();
	return os;
};