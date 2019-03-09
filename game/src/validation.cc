#include "validation.h"
#include <algorithm>
#include <iostream>

using namespace std;

void PrintError(const string& message, const Action& action) {
    cout << "Invalid Action - " << message << " (" << action << ")" << endl;    
}

bool ValidateAction(const BoardState& board_state, const int player_index, const Action& action, const bool print_error) {
    if (action.GetType() == ActionType::MOVE) {
        // Check if move is to a valid location
        auto valid_moves = board_state.GetValidMoves(player_index);
        if (find(valid_moves.begin(), valid_moves.end(), action.GetMovePosition()) == valid_moves.end()) {
            if (print_error)
                PrintError("Move location invalid", action);
            return false;
        }
    }
    else {
        // Player has enough walls
        if (board_state.GetPlayerWallCount(player_index) == 0) {
            if (print_error)
                PrintError("Player has insufficient walls", action);
            return false;
        }
        // Wall is within bounds
        if (!BoardState::IsWallIndexInBounds(action.GetBlockPosition())) {
            if (print_error)
                PrintError("Wall out of bounds", action);
            return false;
        }
        // Wall is not on top of another wall
        if (board_state.GetWall(action.GetBlockPosition()) != 0) {
            if (print_error)
                PrintError("Fully overlapping walls", action);
            return false;
        }
        // Wall is not directly next to another wall of the same orientation
        const auto shift_amt = (action.GetBlockOrientation() == WallOrientation::HORIZONTAL) ? Vectori(1, 0) : Vectori(0, 1);
        const auto adjacent_point_1 = action.GetBlockPosition() - shift_amt;
        const auto adjacent_point_2 = action.GetBlockPosition() + shift_amt;
        if (BoardState::IsWallIndexInBounds(adjacent_point_1)
            && board_state.GetWall(adjacent_point_1) == action.GetBlockOrientation()) {
            if (print_error)
                PrintError("Partially overlapping walls", action);
            return false;
        }            
        if (BoardState::IsWallIndexInBounds(adjacent_point_2)
            && board_state.GetWall(adjacent_point_2) == action.GetBlockOrientation()) {
            if (print_error)
                PrintError("Partially overlapping walls", action);
            return false;
        }
        // Player is not boxed in
        const auto copy = BoardState(board_state, action, player_index);   
        if (IsPlayerTrapped(copy, player_index)) {
            if (print_error)
                PrintError("Player trapped", action);
            return false;
        }
        const auto opponent_index = 1 - player_index;
        if (IsPlayerTrapped(copy, opponent_index)) {
            if (print_error)
                PrintError("Opponent trapped", action);
            return false;
        }
    }
    return true;
}

bool IsValidWall(const BoardState& board_state, const Vectori position, const int orientation){
    if(board_state.GetWall(position) != 0)
        return false;
    const auto shift_amount = orientation == 1 ? Vectori(0, 1) : Vectori(1, 0);
    const auto point_a = position + shift_amount;
    if (BoardState::IsWallIndexInBounds(point_a) && board_state.GetWall(point_a) == orientation)
        return false;
    const auto point_b = position - shift_amount;
    if (BoardState::IsWallIndexInBounds(point_b) && board_state.GetWall(point_b) == orientation)
        return false;
    return true;
}

bool IsPlayerTrapped(const BoardState& board_state, const int player_index) {
    return board_state.GetPlayerDistance(player_index) == -1;
}

bool IsEitherPlayerTrapped(const BoardState& board_state) {
    return IsPlayerTrapped(board_state, 0) || IsPlayerTrapped(board_state, 1);
}