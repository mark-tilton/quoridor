#ifndef QUORIDORBOT_PLAYER_H_
#define QUORIDORBOT_PLAYER_H_

#include "action.h"
#include "board_state.h"
#include <memory>

class Player {
public:
    Player(int id);

    void SetIndex(int index);

    virtual Action TakeAction(const BoardState& board_state) = 0;

    int GetId() const;
    int GetGoalRow() const;

protected:
    std::vector<Vectori> GetValidMoves(const BoardState& board_state) const;

    int id_;
    int index_;
    int opp_index_;
    int goal_row_;
    int opp_goal_row_;
};

#endif