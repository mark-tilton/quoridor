#ifndef QUORIDORBOT_VALIDATION_H_
#define QUORIDORBOT_VALIDATION_H_

#include "board_state.h"
#include "action.h"

bool ValidateAction(const BoardState& board_state, int player_index, const Action& action, bool print_error = false);

#endif