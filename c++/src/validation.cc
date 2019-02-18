#include "validation.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool ValidateAction(const BoardState& board_state, int player_index, const Action& action, bool print_error) {
    if (action.GetType() == ActionType::MOVE) {
        // Check if move is to a valid location
        auto valid_moves = board_state.GetValidMoves(player_index);
        if (find(valid_moves.begin(), valid_moves.end(), action.GetMovePosition()) == valid_moves.end()) {
            if (print_error) 
                cout << "Invalid Action: Move location invalid";
            return false;
        }
    }
    else {
        // Player has enough walls
        if (board_state.GetPlayerWallCount(player_index) == 0) {
            if (print_error) 
                cout << "Invalid Action: Player has insufficient walls";
            return false;
        }
        // Wall is within bounds
        if (!board_state.IsWallIndexInBounds(action.GetBlockPosition())) {
            if (print_error) 
                cout << "Invalid Action: Wall out of bounds";
            return false;
        }
        // Wall is not on top of another wall
        if (board_state.GetWall(action.GetBlockPosition()) != 0) {
            if (print_error)
                cout << "Invalid Action: Overlapping walls";
            return false;
        }
        // Wall is not directly next to another wall of the same orientation
        auto shift_amt = (action.GetBlockOrientation() == WallOrientation::HORIZONTAL) ? Vectori(1, 0) : Vectori(0, 1);
        auto adjacent_point_1 = action.GetBlockPosition() - shift_amt;
        auto adjacent_point_2 = action.GetBlockPosition() + shift_amt;
        if (board_state.IsWallIndexInBounds(adjacent_point_1) 
            && board_state.GetWall(adjacent_point_1) == action.GetBlockOrientation())
            if (print_error) 
                cout << "Invalid Action: Overlapping walls";
            return false;
        if (board_state.IsWallIndexInBounds(adjacent_point_2) 
            && board_state.GetWall(adjacent_point_2) == action.GetBlockOrientation())
            if (print_error) 
                cout << "Invalid Action: Overlapping walls";
            return false;
        // Player is not boxed in
        auto copy = BoardState(board_state);
        copy.SetWall(action.GetBlockPosition(), action.GetBlockOrientation());
        auto player_distance_matrix = copy.GetDistanceMatrix((player_index == 0) ? 8 : 0);
        if (player_distance_matrix[copy.GetPlayerPosition(player_index)] == -1)
            if (print_error) 
                cout << "Invalid Action: Player trapped";
            return false;
        auto opponent_index = 1 - player_index;
        auto opponent_distance_matrix = copy.GetDistanceMatrix((player_index == 0) ? 0 : 8);
        if (opponent_distance_matrix[copy.GetPlayerPosition(opponent_index)] == -1)
            if (print_error) 
                cout << "Invalid Action: Opponent trapped";
            return false;
    }
    return true;
}