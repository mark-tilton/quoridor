#include "board_node.h"
#include "validation.h"

using namespace std;

BoardNode::BoardNode(const BoardState* board_state, Action* action, int player_index) : 
    board_state_(board_state),
    action_(action) {
        player_index_ = player_index;
        opp_index_ = 1 - player_index;
        auto a_pos = board_state->GetPlayerPosition(0);
        auto b_pos = board_state->GetPlayerPosition(1);
        player_pos_ = player_index == 0 ? a_pos : b_pos;
        opp_pos_ = player_index == 0 ? b_pos : a_pos;
        player_goal_ = player_index == 0 ? 8 : 0;
        opp_goal_ = player_index == 0 ? 0 : 8;
        player_walls_ = board_state->GetPlayerWallCount(player_index_);
        opp_walls_ = board_state->GetPlayerWallCount(opp_index_);
        score_ = 0;
        children_ = vector<BoardNode*>();
}

BoardNode::~BoardNode() {
    delete board_state_;
    delete action_;
    for(auto child : children_) {
        delete child;
    }
}

void BoardNode::BuildChildren() {
    // If we are already at the end, there are no valid moves.
    auto player_dist = board_state_->GetDistanceMatrix(player_index_)[player_pos_];
    if (player_dist == 0)
        return;

    // Add all the valid movement positions to the valid moves.
    vector<Action*> valid_actions;
    auto valid_moves = board_state_->GetValidMoves(player_pos_, opp_pos_);
    for (auto move : valid_moves) {
        valid_actions.push_back(new Action(move));
    }

    if (player_walls_ > 0) {
        auto deviation_matrix = board_state_->CalculateDeviationMatrix(board_state_->GetDistanceMatrix(opp_index_), opp_pos_, 7);
        // Get a subset of wall placements we want to consider.
        // For each column
        for (int x  = 0; x < 8; x++) {
            // For each row
            for (int y = 0; y < 8; y++) {
                Vectori pos(x, y);
                // For each orientation
                for (int o = 1; o <= 2; o++) {
                    // If this is a valid place to put a wall.
                    if (IsValidWall(*board_state_, pos, o)) {
                        // Figure out which cells this blocks.
                        auto blocked_paths = BoardState::GetBlockedPaths(pos, o);
                        bool blocks_shortest_path = false;
                        // Each wall blocks 2 paths, since they are 2 cells wide.
                        for(int i = 0; i < 2; i++) {
                            // Get the points in front and behind the wall.
                            auto point_a = blocked_paths[i][0];
                            auto point_b = blocked_paths[i][1];
                            if ((board_state_->IsCellIndexInBounds(point_a) 
                                and board_state_->IsCellIndexInBounds(point_b))) {
                                // If the cells behind and in front of the wall are in the shortest path,
                                // we know this wall blocks along the shortest path.
                                blocks_shortest_path = blocks_shortest_path
                                    || (deviation_matrix[point_a] == 0 
                                    && deviation_matrix[point_b] == 0);
                            }
                        }
                        if(blocks_shortest_path)
                            valid_actions.push_back(new Action(pos, (WallOrientation)o));
                    }
                }
            }
        }                
    }

    // Create a board state for each of the valid actions.
    for (auto action : valid_actions) {
        auto new_board_state = new BoardState(*board_state_, *action, player_index_);
        if (!(IsEitherPlayerTrapped(*new_board_state))) {
            auto child_node = new BoardNode(new_board_state, action, opp_index_);
            children_.push_back(child_node);
        }
    }
}

void BoardNode::CalculateScore(bool minimizing, int player_index) {
    const auto opp_index = 1-player_index;
    if (children_.empty()) {
        // When the board has no children calculate the distances from the end for each player.
        const auto opp_dist = board_state_->GetDistanceMatrix(opp_index)[board_state_->GetPlayerPosition(opp_index)];
        const auto player_dist = board_state_->GetDistanceMatrix(player_index)[board_state_->GetPlayerPosition(player_index)];
        score_ = (opp_dist - player_dist);
    }
    else {
        auto set_score = false;
        auto best_score = 0;
        for (auto child : children_) {
            child->CalculateScore(!minimizing, player_index);
            const auto score = child->score_;
            if(!set_score || (minimizing && score < best_score) || (!minimizing && score > best_score)) {
                best_child_ = child;
                best_score = score;
                set_score = true;
            }
        }
        score_ = best_score;
    }
}

vector<BoardNode*> BoardNode::GetChildren() {
    return children_;
}

BoardNode* BoardNode::GetBestChild() {
    return best_child_;
}

Action* BoardNode::GetAction() {
    return action_;
}