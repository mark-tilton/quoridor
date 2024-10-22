#include "player.h"

Player::Player(const int id) : id_(id) {
}

void Player::SetIndex(const int index) {
    index_ = index;
    opp_index_ = (index == 0) ? 1 : 0;
    goal_row_ = (index == 0) ? 8 : 0;
    opp_goal_row_ = (index == 0) ? 0 : 8;
}

int Player::GetId() const {
    return id_;
}

int Player::GetGoalRow() const {
    return goal_row_;
}

std::vector<Vectori> Player::GetValidMoves(const BoardState& board_state) const {
    return board_state.GetValidMoves(index_);
}