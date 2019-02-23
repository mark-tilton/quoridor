#ifndef QUORIDORBOT_TURN_H_
#define QUORIDORBOT_TURN_H_

#include "board_state.h"
#include "action.h"

class Turn {
public:
    Turn(const BoardState& board_state, int player_index, const Action& action);

    BoardState GetBoardState() const;

private:
    BoardState board_state_;
    int player_index_;
    Action action_;
};

#endif