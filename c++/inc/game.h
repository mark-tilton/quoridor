#ifndef QUORIDORBOT_GAME_H_
#define QUORIDORBOT_GAME_H_

#include "player.h"
#include "turn.h"
#include <vector>
#include <optional>

class Game {
public:
    Game(Player* player_1, Player* player_2);

    void Play();
    bool TakeTurn();
    void Reset();

    int GetWinner();

private:
    int current_player_index_;
    int winning_player_index_;
    std::array<Player*, 2> players_;
    std::vector<Turn> turns_;
    BoardState current_board_;

};

#endif