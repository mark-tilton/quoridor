#include "minimax_player.h"
#include "board_node.h"
#include <limits>
#include "random_player.h"

using namespace std;

MinimaxPlayer::MinimaxPlayer(const int depth, const double future_value) : Player(3), branch_depth_(depth), future_value_(future_value) {
}

Action MinimaxPlayer::TakeAction(const BoardState& board_state) {
    //if(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < 0.1) {
    //    RandomPlayer rPlayer;
    //    rPlayer.SetIndex(index_);
    //    return rPlayer.TakeAction(board_state);
    //}
    auto current_node = BoardNode(board_state, index_, future_value_);
    current_node.BuildChildren(branch_depth_, index_, true, -numeric_limits<double>::infinity(), numeric_limits<double>::infinity());    
    return current_node.GetBestAction();
}