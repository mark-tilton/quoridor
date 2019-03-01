#include "shortest_path_player.h"
#include "validation.h"
#include <random>

using namespace std;

ShortestPathPlayer::ShortestPathPlayer() : Player(2) {
}

Action ShortestPathPlayer::TakeAction(const BoardState& board_state) {
    while (true)
    {
        if (rand() % 2 > 0 && board_state.GetPlayerWallCount(index_) > 0) {
            // Block along opponent's shortest path            
            const auto& distance_matrix = board_state.GetDistanceMatrix(opp_index_);
            auto opp_position = board_state.GetPlayerPosition(opp_index_);
            auto best_move = GetBestMove(board_state, opp_index_, distance_matrix);
            auto direction = best_move - opp_position;
            const auto orientation = direction.y == 0 ? WallOrientation::VERTICAL : WallOrientation::HORIZONTAL;
            Vectori points[2];
            BoardState::GetWallPoints(opp_position, direction, points[0], points[1]);
            for (const auto point : points) {
                auto action = Action(point, orientation);
                if (ValidateAction(board_state, index_, action)) {
                    return action;
                }
            }
        }
        else {
            // Move along shortest path
            const auto& distance_matrix = board_state.GetDistanceMatrix(index_);
            const auto best_move = GetBestMove(board_state, index_, distance_matrix);
            return Action(best_move);
        }
    }
}

Vectori ShortestPathPlayer::GetBestMove(
    const BoardState& board_state,
    const int player_index,
    const Matrix& distance_matrix) {
    auto best_distance = -1;
    auto best_move = Vectori(-1, -1);
    for (const auto position : board_state.GetValidMoves(player_index)) {
	    const auto distance = distance_matrix[position];
        if (best_distance == -1 || distance < best_distance) {
            best_distance = distance;
            best_move = position;
        }
    }
    return best_move;
}
