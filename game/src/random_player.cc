#include "random_player.h"
#include "validation.h"
#include <random>

RandomPlayer::RandomPlayer() : Player(1) {
}

Action RandomPlayer::TakeAction(const BoardState& board_state) {
    while (true)
    {
        if (rand() % 4 > 0) {
            auto valid_moves = GetValidMoves(board_state);
            auto action = Action(valid_moves[rand() % valid_moves.size()]);
            if (ValidateAction(board_state, index_, action)) {
                return action;
            }
        }
        else {
            auto wall_pos = Vectori(rand() % 8, rand() % 8);
            auto wall_orientation = rand() % 2 + 1;
            auto action = Action(wall_pos, static_cast<WallOrientation>(wall_orientation));
            if (ValidateAction(board_state, index_, action)) {
                return action;
            }
        }
    }
}
