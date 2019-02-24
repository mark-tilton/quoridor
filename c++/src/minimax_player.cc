#include "minimax_player.h"
#include "board_node.h"
#include <vector>

using namespace std;

MinimaxPlayer::MinimaxPlayer(int index) : Player(index) {

}

Action MinimaxPlayer::TakeAction(const BoardState& board_state) {
        auto current_node = BoardNode(board_state, Action(Vectori(0, 0)), index_);

        vector<BoardNode> nodes_to_process;
        vector<BoardNode> child_nodes;
        child_nodes.push_back(current_node);
        for (int i = 0; i < branch_depth_; i++) {
            nodes_to_process.swap(child_nodes);
            child_nodes.clear();
            for (auto node : nodes_to_process) {
                node.BuildChildren();
                child_nodes.push_back(node.children);
            }
        }

        current_node.CalculateScore();

        return current_node.GetBestChild().GetAction();
}