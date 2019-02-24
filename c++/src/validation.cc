#include "validation.h"
#include <algorithm>
#include <iostream>

using namespace std;

void PrintError(string message, Action action) {
    cout << "Invalid Action - " << message << " (" << action << ")" << endl;    
}

bool ValidateAction(const BoardState& board_state, int player_index, const Action& action, bool print_error) {
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
        if (!board_state.IsWallIndexInBounds(action.GetBlockPosition())) {
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
        auto shift_amt = (action.GetBlockOrientation() == WallOrientation::HORIZONTAL) ? Vectori(1, 0) : Vectori(0, 1);
        auto adjacent_point_1 = action.GetBlockPosition() - shift_amt;
        auto adjacent_point_2 = action.GetBlockPosition() + shift_amt;
        if (board_state.IsWallIndexInBounds(adjacent_point_1)
            && board_state.GetWall(adjacent_point_1) == action.GetBlockOrientation()) {
            if (print_error)
                PrintError("Partially overlapping walls", action);
            return false;
        }            
        if (board_state.IsWallIndexInBounds(adjacent_point_2) 
            && board_state.GetWall(adjacent_point_2) == action.GetBlockOrientation()) {
            if (print_error)
                PrintError("Partially overlapping walls", action);
            return false;
        }
        // Player is not boxed in
        auto copy = BoardState(board_state);
        copy.SetWall(action.GetBlockPosition(), action.GetBlockOrientation());       
        if (IsPlayerTrapped(copy, player_index)) {
            if (print_error)
                PrintError("Player trapped", action);
            return false;
        }
        auto opponent_index = 1 - player_index;
        if (IsPlayerTrapped(copy, opponent_index)) {
            if (print_error)
                PrintError("Opponent trapped", action);
            return false;
        }
    }
    return true;
}

bool IsValidWall(const BoardState& board_state, const Vectori position, int orientation){
    if(board_state.GetWall[position] != 0)
        return false;
    auto shift_amount = orientation == 1 ? Vectori(0, 1) : Vectori(1, 0);
    auto point_a = position + shift_amount;
    if (board_state.IsWallIndexInBounds(point_a) && board_state.GetWall[point_a] == orientation)
        return false;
    auto point_b = position - shift_amount;
    if (board_state.IsWallIndexInBounds(point_b) && board_state.GetWall[point_b] == orientation)
        return false;
    return true;
}

bool IsPlayerTrapped(const BoardState& board_state, int player_index) {
    return board_state.GetDistanceMatrix(player_index)[board_state.GetPlayerPosition(player_index)] == -1;
}

bool IsEitherPlayerTrapped(const BoardState& board_state) {
    return IsPlayerTrapped(board_state, 0) || IsPlayerTrapped(board_state, 1);
}