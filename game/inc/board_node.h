#ifndef QUORIDORBOT_BOARD_NODE_H_
#define QUORIDORBOT_BOARD_NODE_H_

#include "vectori.h"
#include "board_state.h"
#include "action.h"

class BoardNode {

public:
    BoardNode(const BoardState& board_state, int player_index);
    void BuildChildren(int depth, int scoring_player, bool maximizing, int alpha, int beta);
    Action GetBestAction() const;

private:
    const BoardState& board_state_;
    Action action_;
    int player_index_;
    int opp_index_;
    Vectori player_pos_;
    Vectori opp_pos_;
    int player_walls_;
    int score_;
};

#endif