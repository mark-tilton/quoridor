#ifndef QUORIDORBOT_MINIMAX_PLAYER_H_
#define QUORIDORBOT_MINIMAX_PLAYER_H_

#include "vectori.h"
#include "board_state.h"
#include "player.h"

class MinimaxPlayer : Player {

public:
    MinimaxPlayer(int index);
    virtual Action TakeAction(const BoardState& board_state) override;

private:
    const int branch_depth_ = 1;

};

#endif