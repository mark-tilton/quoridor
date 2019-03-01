#ifndef QUORIDORBOT_RANDOMPLAYER_H_
#define QUORIDORBOT_RANDOMPLAYER_H_

#include "player.h"

class RandomPlayer : public Player {
public:
    RandomPlayer();

    virtual Action TakeAction(const BoardState& board_state) override;
};

#endif