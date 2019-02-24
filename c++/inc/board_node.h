#ifndef QUORIDORBOT_BOARD_NODE_H_
#define QUORIDORBOT_BOARD_NODE_H_

#include "vectori.h"
#include "board_state.h"
#include "action.h"

class BoardNode {

public:
    BoardNode(const BoardState& board_state, Action action, int player_index);
    void BuildChildren();
    void CalculateScore();
    BoardNode& GetBestChild();
    Action& GetAction();

private:
    const BoardState& board_state_;
    Action action_;
    int player_index_;
    int opp_index_;
    Vectori player_pos_;
    Vectori opp_pos_;
    int player_goal_;
    int opp_goal_;
    int player_walls_;
    int opp_walls_;
    vector<BoardNode&> children_;
    int score_;
    BoardNode& best_child_;
};

#endif