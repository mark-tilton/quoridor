#include "turn.h"

Turn::Turn(
    const BoardState& board_state, int player_index, const Action& action) :
    board_state_(board_state),
    player_index_(player_index),
    action_(action) {
}

BoardState Turn::GetBoardState() const {
    return board_state_;
}
