#ifndef QUORIDORBOT_TURN_H_
#define QUORIDORBOT_TURN_H_

#include "board_state.h"
#include "action.h"

class Turn {
public:
    Turn(const BoardState& initial_board_state, const BoardState& resulting_board_state, int player_index, const Action& action);

    BoardState GetInitialBoardState() const;
    BoardState GetResultingBoardState() const;

private:
    BoardState initial_board_state_;
    BoardState resulting_board_state_;
    int player_index_;
    Action action_;
};

#endif