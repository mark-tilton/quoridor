#ifndef QUORIDORBOT_MINIMAX_PLAYER_H_
#define QUORIDORBOT_MINIMAX_PLAYER_H_

#include "vectori.h"
#include "board_state.h"
#include "player.h"

class MinimaxPlayer : public Player {

public:
    MinimaxPlayer(int depth);
    virtual Action TakeAction(const BoardState& board_state) override;

private:
    int branch_depth_;

};

#endif