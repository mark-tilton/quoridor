#ifndef QUORIDORBOT_RANDOMPLAYER_H_
#define QUORIDORBOT_RANDOMPLAYER_H_

#include "player.h"

class RandomPlayer : public Player {
public:
    RandomPlayer(double move_chance = 0.75);

    virtual Action TakeAction(const BoardState& board_state) override;

private:
    double move_chance_;

};

#endif