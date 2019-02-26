#ifndef QUORIDORBOT_BOARD_NODE_H_
#define QUORIDORBOT_BOARD_NODE_H_

#include <vector>
#include "vectori.h"
#include "board_state.h"
#include "action.h"

class BoardNode {

public:
    BoardNode(const BoardState* board_state, Action* action, int player_index);
    ~BoardNode();
    void BuildChildren();
    void CalculateScore(bool minimizing, int player_index);
    std::vector<BoardNode*> GetChildren();
    BoardNode* GetBestChild();
    Action* GetAction();

private:
    const BoardState* board_state_;
    Action* action_;
    int player_index_;
    int opp_index_;
    Vectori player_pos_;
    Vectori opp_pos_;
    int player_goal_;
    int opp_goal_;
    int player_walls_;
    int opp_walls_;
    std::vector<BoardNode*> children_;
    int score_;
    BoardNode* best_child_ = nullptr;
};

#endif