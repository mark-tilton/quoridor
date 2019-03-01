#include "minimax_player.h"
#include "board_node.h"
#include <limits>

using namespace std;

MinimaxPlayer::MinimaxPlayer(const int depth) : Player(3), branch_depth_(depth) {
}

Action MinimaxPlayer::TakeAction(const BoardState& board_state) {
    auto current_node = BoardNode(board_state, index_);
    const auto alpha = -numeric_limits<int>::max();
    const auto beta = numeric_limits<int>::max();

    current_node.BuildChildren(branch_depth_, index_, true, alpha, beta);    
    return current_node.GetBestAction();
}