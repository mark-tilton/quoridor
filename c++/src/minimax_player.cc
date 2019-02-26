#include "minimax_player.h"
#include "board_node.h"
#include <vector>
#include <limits>

using namespace std;

MinimaxPlayer::MinimaxPlayer(int depth) : Player(3), branch_depth_(depth) {

}

Action MinimaxPlayer::TakeAction(const BoardState& board_state) {
    auto current_node = BoardNode(board_state, index_);
    int alpha = -std::numeric_limits<int>::max();
    int beta = std::numeric_limits<int>::max();

    current_node.BuildChildren(5, index_, true, alpha, beta);    
    return current_node.GetBestAction();
}