#ifndef QUORIDORBOT_MINIMAX_PLAYER_H_
#define QUORIDORBOT_MINIMAX_PLAYER_H_

#include "vectori.h"
#include "board_state.h"
#include "player.h"

class MinimaxPlayer : public Player {

public:
    MinimaxPlayer(int depth, double future_value, double random_chance);
    virtual Action TakeAction(const BoardState& board_state) override;

private:
    int branch_depth_;
    double future_value_;
    double random_chance_;
};

#endif