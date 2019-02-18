#include "player.h"

Player::Player(int id, int index) : id_(id), index_(index) {
    opp_index_ = (index == 0) ? 1 : 0;
    goal_row_ = (index == 0) ? 8 : 0;
    opp_goal_row_ = (index == 0) ? 0 : 8;
}

int Player::GetGoalRow() {
    return goal_row_;
}

std::vector<Vectori> Player::GetValidMoves(const BoardState& board_state) const {
    return board_state.GetValidMoves(index_);
}