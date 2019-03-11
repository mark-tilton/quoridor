#include "mcts_player.h"
#include "mcts_node.h"
#include "validation.h"
#include "game.h"
#include "shortest_path_player.h"
#include <limits>
#include <chrono>
#include <iostream>
#include <fstream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace std;
using namespace chrono;
using namespace rapidjson;

MctsPlayer::MctsPlayer(long long time_out) : 
	Player(4),
	time_out_(time_out) {

}

MctsNode* SelectNode(MctsNode& node, bool maximizing) {
	const auto c = sqrt(2);
	auto max_score = -numeric_limits<double>::infinity();
	MctsNode* max_node = nullptr;
	const auto parent_visits = node.GetVisitCount();
	for (auto& child : node) {
		const auto visits = child.GetVisitCount();
		auto child_score = child.GetScore();
		child_score = maximizing ? child_score : -child_score;
		const auto score = child_score / visits + c * sqrt(log(parent_visits) / visits);
		if (score > max_score) {
			max_score = score;
			max_node = &child;
		}
	}
	return max_node;
}

void Backpropagate(MctsNode& node, double score) {
	node.Visit(score);
	if (node.GetParent()) {
		Backpropagate(*node.GetParent(), score);
	}
}

void ExpandNode(MctsNode* node, const int player_index) {
	const auto board_state = node->GetBoardState();
	const auto opp_index = 1 - player_index;
	const auto player_pos = board_state.GetPlayerPosition(player_index);
	const auto opp_pos = board_state.GetPlayerPosition(opp_index);
	const auto player_walls = board_state.GetPlayerWallCount(player_index);
    const auto player_dist = board_state.GetPlayerDistance(player_index);
    const auto opp_dist = board_state.GetPlayerDistance(opp_index);
	if (player_dist == 0) {
        Backpropagate(*node, -opp_dist);
		return;
	}
	if (opp_dist == 0) {
        Backpropagate(*node, player_dist);
		return;
	}

	// Add all the valid movement positions to the valid moves.
	vector<Action> valid_actions;
	auto valid_moves = board_state.GetValidMoves(player_pos, opp_pos);
	valid_actions.reserve(valid_moves.size());
	valid_actions.insert(valid_actions.end(), valid_moves.begin(), valid_moves.end());

	if (player_walls > 0) {
		// For each column
		for (auto x = 0; x < 8; x++) {
			// For each row
			for (auto y = 0; y < 8; y++) {
				Vectori pos(x, y);
				// For each orientation
				for (auto o = 1; o <= 2; o++) {
					// If this is a valid place to put a wall.
					if (IsValidWall(board_state, pos, o)) {
						valid_actions.emplace_back(pos, WallOrientation(o));
					}
				}
			}
		}
	}

	for (auto action : valid_actions) {
		auto new_board_state = BoardState(node->GetBoardState(), action, player_index);
		if (IsEitherPlayerTrapped(new_board_state))
			continue;
		auto board_node = MctsNode(new_board_state, action, node);
		node->Expand(board_node);
	}
}

double ScoreNode(const MctsNode& node, int player_index) {
    const auto& board_state = node.GetBoardState();

	//auto player = ShortestPathPlayer();
	//auto opponent = ShortestPathPlayer();
	//auto game = Game(&player, &opponent, false, false, board_state);
	//game.Play();
	//return (game.GetWinner() == player_index) ? 1 : -1;

	const auto player_dist = board_state.GetPlayerDistance(player_index);
	const auto opp_dist = board_state.GetPlayerDistance(1 - player_index);
	return (opp_dist - player_dist);
}

Action MctsPlayer::TakeAction(const BoardState& board_state) {
	const auto start_time = high_resolution_clock::now();
	auto root_node = MctsNode(board_state, nullopt, nullptr);

	auto* selected_node = &root_node;
	auto selected_index = index_;
	auto n = 0;
	//while (n < 10) {
		n++;
	while (duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count() < time_out_) {
		// Expand
		ExpandNode(selected_node, selected_index);

		// Simulate
		auto scores = vector<double>(selected_node->GetChildCount());
		//scores.reserve(selected_node->GetChildCount());
		//#pragma omp parallel for
		for(auto i = 0; i < selected_node->GetChildCount(); i++) {
			auto& child = (*selected_node)[i];
			const auto score = ScoreNode(child, index_);
			scores[i] = score;
		}

		// Backpropagate
		for (auto i = 0; i < selected_node->GetChildCount(); i++) {
			Backpropagate((*selected_node)[i], scores[i]);
		}
		
		// Select next
		selected_node = &root_node;
		selected_index = index_;
		while (selected_node->GetChildCount() > 0) {
			selected_node = SelectNode(*selected_node, selected_index == index_);
			selected_index = 1 - selected_index;
		}
	}

	auto max_visit_count = -numeric_limits<double>::infinity();
	auto best_action = 0;
	for (auto i = 0; i < root_node.GetChildCount(); i++) {
		auto& child = root_node[i];
		const auto visit_count = child.GetVisitCount();
		if (visit_count > max_visit_count) {
			max_visit_count = visit_count;
			best_action = i;
		}
	}
	
	//StringBuffer sb;
	//Writer<StringBuffer> writer(sb);	
	//root_node.Serialize(writer);
	//ofstream file;
	//file.open("turn.json", std::ios_base::trunc);
	//file << sb.GetString();
	//file.close();

	return root_node[best_action].GetAction().value();
}