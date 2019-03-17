#include "board_node.h"
#include "validation.h"
#include <limits>
#include <vector>

using namespace std;

BoardNode::BoardNode(const BoardState& board_state, const int player_index, const double future_value) : 
    board_state_(board_state),
    action_(Action(Vectori(0, 0))),
    future_value_(future_value) {
        player_index_ = player_index;
        opp_index_ = 1 - player_index;
        auto a_pos = board_state.GetPlayerPosition(0);
        auto b_pos = board_state.GetPlayerPosition(1);
        player_pos_ = player_index == 0 ? a_pos : b_pos;
        opp_pos_ = player_index == 0 ? b_pos : a_pos;
        player_walls_ = board_state.GetPlayerWallCount(player_index_);
        score_ = 0;
}

void BoardNode::BuildChildren(int depth, int scoring_player, bool maximizing, double alpha, double beta) {

    const auto opp_index = 1-scoring_player;
    const auto opp_dist = board_state_.GetPlayerDistance(opp_index);
    const auto player_dist = board_state_.GetPlayerDistance(scoring_player);
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
        // For each column
        for (auto x  = 0; x < 8; x++) {
            // For each row
            for (auto y = 0; y < 8; y++) {
                Vectori pos(x, y);
                // For each orientation
                for (auto o = 1; o <= 2; o++) {
                    // If this is a valid place to put a wall.
                    if (IsValidWall(board_state_, pos, o)) {
                        valid_actions.emplace_back(pos, WallOrientation(o));
                    }
                }
            }
        }                
    }

	auto actions = vector<pair<int, int>>();
	actions.reserve(valid_actions.size());
	auto board_states = vector<BoardState>();
	board_states.reserve(valid_actions.size());
	for (auto i = 0; i < valid_actions.size(); i++) {
		const auto& action = valid_actions[i];
		board_states.emplace_back(board_state_, action, player_index_);
		const auto& board_state = board_states[i];
		const auto opp_dist_1 = board_state.GetPlayerDistance(opp_index);
		const auto player_dist_1 = board_state.GetPlayerDistance(scoring_player);
		const auto score = opp_dist_1 - player_dist_1;
		actions.emplace_back(i, score);
	}

	if (maximizing)
	{
		sort(actions.begin(), actions.end(), [](const pair<int, int> & a, const pair<int, int> & b) -> bool
			{
				return a.second > b.second;
			});
	}
	else
	{
		sort(actions.begin(), actions.end(), [](const pair<int, int> & a, const pair<int, int> & b) -> bool
			{
				return a.second < b.second;
			});
		
	}
		
    auto value = maximizing ? -numeric_limits<int>::max() : numeric_limits<int>::max();
	for (auto i = 0; i < valid_actions.size(); i++) {	
		const auto& action = valid_actions[actions[i].first];
        const auto& new_board_state = board_states[actions[i].first];
        if (!IsEitherPlayerTrapped(new_board_state)) {
            auto child_node = BoardNode(new_board_state, opp_index_, future_value_);            
            child_node.BuildChildren(depth - 1, scoring_player, !maximizing, a, b);
            if(maximizing) {
                const auto child_score = child_node.score_;
                if(child_score > value) {
                    value = child_score;
                    action_ = action;
                }
                a = max(a, value);
                if(a >= b)
                    break;
            } 
            else {
                const auto child_score = child_node.score_ * future_value_ + (opp_dist - player_dist) * (1 - future_value_);
                if(child_score < value) {
                    value = child_score;
                    action_ = action;
                }
                b = min(b, value);
                if(a >= b)
                    break;
            }
        }
    }
    score_ = value;
}

Action BoardNode::GetBestAction() const {
    return action_;
}
