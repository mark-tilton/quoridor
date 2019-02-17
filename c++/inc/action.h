#ifndef QUORIDORBOT_ACTION_H_
#define QUORIDORBOT_ACTION_H_

#include "vectori.h"
#include "board_state.h"

enum ActionType {
    MOVE = 0,
    BLOCK = 1,
};

class Action {
public:
    Action(Vectori move_position);
    Action(Vectori block_position, WallOrientation block_orientation);

    virtual void Apply(BoardState& board_state, int player_index) const;

    ActionType GetType() const;
    Vectori GetMovePosition() const;
    Vectori GetBlockPosition() const;
    WallOrientation GetBlockOrientation() const;

protected:
    ActionType type_;
    Vectori move_position_;
    Vectori block_position_;
    WallOrientation block_orientation_;
};

std::ostream &operator<< (std::ostream &os, const Action &a);

#endif