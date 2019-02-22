#include "turn.h"

Turn::Turn(
    const BoardState& initial_board_state, 
    const BoardState& resulting_board_state, 
    int player_index, 
    const Action& action) :
    initial_board_state_(initial_board_state),
    resulting_board_state_(resulting_board_state),
    player_index_(player_index),
    action_(action) {
}

BoardState Turn::GetInitialBoardState() const {
    return initial_board_state_;
}

BoardState Turn::GetResultingBoardState() const {
    return resulting_board_state_;
}
