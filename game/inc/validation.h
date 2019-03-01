#ifndef QUORIDORBOT_VALIDATION_H_
#define QUORIDORBOT_VALIDATION_H_

#include "board_state.h"
#include "action.h"

bool ValidateAction(const BoardState& board_state, int player_index, const Action& action, bool print_error = false);
bool IsValidWall(const BoardState& board_state, const Vectori position, int orientation);
bool IsPlayerTrapped(const BoardState& board_state, int player_index);
bool IsEitherPlayerTrapped(const BoardState& board_state);

#endif