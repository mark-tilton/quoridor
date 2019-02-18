#include "action.h"
#include <stdexcept>

Action::Action(Vectori move_position) : 
    type_(ActionType::MOVE),
    move_position_(move_position) {
}

Action::Action(Vectori block_position, WallOrientation block_orientation) : 
    type_(ActionType::BLOCK),
    block_position_(block_position),
    block_orientation_(block_orientation) {
    if (block_orientation == WallOrientation::NONE) {
        throw std::invalid_argument("NONE is an invalid wall orientation.");
    }
}

void Action::Apply(BoardState& board_state, int player_index) const {
    if (type_ == ActionType::MOVE) { 
        board_state.SetPlayerPosition(player_index, move_position_);
    }
    else {
        board_state.SetWall(block_position_, block_orientation_);
        board_state.SetPlayerWallCount(player_index, (board_state.GetPlayerWallCount(player_index) - 1));
    }    
}

ActionType Action::GetType() const {
    return type_;
}

Vectori Action::GetMovePosition() const {
    return move_position_;
}

Vectori Action::GetBlockPosition() const {
    return block_position_;
}

WallOrientation Action::GetBlockOrientation() const {
    return block_orientation_;
}

std::ostream &operator<< (std::ostream &os, const Action &a) {
    if (a.GetType() == ActionType::MOVE) {
        os << "Move: " << a.GetMovePosition();
    }
    else {
        os << "Block: " << a.GetBlockPosition();
        os << ", " << ((a.GetBlockOrientation() == HORIZONTAL) ? "Horizontal" : "Vertical");
    }
    return os;
};