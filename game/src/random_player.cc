#include "random_player.h"
#include "validation.h"
#include <random>

RandomPlayer::RandomPlayer(double move_chance) : 
    Player(1),
    move_chance_(move_chance) {
}

Action RandomPlayer::TakeAction(const BoardState& board_state) {
    while (true) {
        if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < move_chance_ 
        || board_state.GetPlayerWallCount(index_) == 0) {
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
