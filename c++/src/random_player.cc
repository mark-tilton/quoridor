#include "random_player.h"
#include <random>

RandomPlayer::RandomPlayer(int id, int index) : Player(id, index) {
}

Action RandomPlayer::TakeAction(const BoardState& board_state) {
    if (rand() % 4 > 0) {
        auto valid_moves = GetValidMoves(board_state);
        return Action(valid_moves[rand() % valid_moves.size()]);
    }
    else {
        auto wall_pos = Vectori(rand() % 8, rand() % 8);
        auto wall_orientation = rand() % 2 + 1;
        return Action(wall_pos, (WallOrientation)wall_orientation);
    }
}
