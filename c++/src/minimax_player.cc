#include "minimax_player.h"
#include "board_node.h"
#include <vector>

using namespace std;

MinimaxPlayer::MinimaxPlayer() : Player(2) {

}

Action MinimaxPlayer::TakeAction(const BoardState& board_state) {
    auto current_node = BoardNode(new BoardState(board_state), nullptr, index_);

    vector<BoardNode*> nodes_to_process;
    vector<BoardNode*> child_nodes;
    child_nodes.push_back(&current_node);
    for (int i = 0; i < 2; i++) {
        nodes_to_process.swap(child_nodes);
        child_nodes.clear();
        for (auto node : nodes_to_process) {
            node->BuildChildren();
            for (auto blah : node->GetChildren()) {
                child_nodes.push_back(blah);
            }
        }
    }

    current_node.CalculateScore();

    return *current_node.GetBestChild()->GetAction();
}