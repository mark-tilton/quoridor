#include "board_node.h"
#include "validation.h"
#include <limits>
#include <vector>

using namespace std;

BoardNode::BoardNode(const BoardState& board_state, const int player_index) : 
    board_state_(board_state),
    action_(Action(Vectori(0, 0))) {
        player_index_ = player_index;
        opp_index_ = 1 - player_index;
        auto a_pos = board_state.GetPlayerPosition(0);
        auto b_pos = board_state.GetPlayerPosition(1);
        player_pos_ = player_index == 0 ? a_pos : b_pos;
        opp_pos_ = player_index == 0 ? b_pos : a_pos;
        player_walls_ = board_state.GetPlayerWallCount(player_index_);
        score_ = 0;
}

void BoardNode::BuildChildren(int depth, int scoring_player, bool maximizing, int alpha, int beta) {

    const auto opp_index = 1-scoring_player;
    const auto opp_dist = board_state_.GetDistanceMatrix(opp_index)[board_state_.GetPlayerPosition(opp_index)];
    const auto player_dist = board_state_.GetDistanceMatrix(scoring_player)[board_state_.GetPlayerPosition(scoring_player)];
    if (player_dist == 0 || opp_dist == 0 || depth == 0) {
        // When the board has no children calculate the distances from the end for each player.
        score_ = opp_dist - player_dist;
        return;
    }

    // Add all the valid movement positions to the valid moves.
    vector<Action> valid_actions;
	auto valid_moves = board_state_.GetValidMoves(player_pos_, opp_pos_);
	valid_actions.reserve(valid_moves.size());
	valid_actions.insert(valid_actions.end(), valid_moves.begin(), valid_moves.end());

    if (player_walls_ > 0) {
        auto deviation_matrix = board_state_.CalculateDeviationMatrix(board_state_.GetDistanceMatrix(opp_index_), opp_pos_, 81);
        // Get a subset of wall placements we want to consider.
        // For each column
        for (auto x  = 0; x < 8; x++) {
            // For each row
            for (auto y = 0; y < 8; y++) {
                Vectori pos(x, y);
                // For each orientation
                for (auto o = 1; o <= 2; o++) {
                    // If this is a valid place to put a wall.
                    if (IsValidWall(board_state_, pos, o)) {
                        // Figure out which cells this blocks.
                        auto blocked_paths = BoardState::GetBlockedPaths(pos, o);
                        auto blocks_shortest_path = false;
                        // Each wall blocks 2 paths, since they are 2 cells wide.
                        for(auto i = 0; i < 2; i++) {
                            // Get the points in front and behind the wall.
                            auto point_a = blocked_paths[i][0];
                            auto point_b = blocked_paths[i][1];
                            if ((BoardState::IsCellIndexInBounds(point_a) 
                                && BoardState::IsCellIndexInBounds(point_b))) {
                                // If the cells behind and in front of the wall are in the shortest path,
                                // we know this wall blocks along the shortest path.
                                blocks_shortest_path = blocks_shortest_path
                                    || (deviation_matrix[point_a] == 0 
                                    && deviation_matrix[point_b] == 0);
                            }
                        }
                        if(blocks_shortest_path)
                            valid_actions.emplace_back(pos, WallOrientation(o));
                    }
                }
            }
        }                
    }

    auto value = maximizing ? -numeric_limits<int>::max() : numeric_limits<int>::max();
    for(auto action : valid_actions) {
        auto new_board_state = BoardState(board_state_, action, player_index_);
        if (!IsEitherPlayerTrapped(new_board_state)) {
            auto child_node = BoardNode(new_board_state, opp_index_);            
            child_node.BuildChildren(depth - 1, scoring_player, !maximizing, alpha, beta);
            if(maximizing) {
                if(child_node.score_ > value) {
                    value = child_node.score_;
                    action_ = action;
                }
                alpha = max(alpha, value);
                if(alpha >= beta)
                    break;
            } else {
                if(child_node.score_ < value) {
                    value = child_node.score_;
                    action_ = action;
                }
                beta = min(beta, value);
                if(alpha >= beta)
                    break;
            }
        }
    }
    score_ = value;
}

Action BoardNode::GetBestAction() const {
    return action_;
}
