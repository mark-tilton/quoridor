#ifndef QUORIDORBOT_GAME_H_
#define QUORIDORBOT_GAME_H_

#include "player.h"
#include "turn.h"
#include <vector>

class Game {
public:
    Game(Player* player_1, Player* player_2);

    void Play();
    bool TakeTurn();
    void Reset();

    int GetWinner() const;
    Turn GetTurn(int index) const;
    int GetTurnCount() const;

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

            writer.Key("player_1_id");
            writer.Int(players_[0]->GetId());

            writer.Key("player_2_id");
            writer.Int(players_[1]->GetId());

            writer.Key("winner");
            writer.Int(winning_player_index_);

            writer.Key(("turns"));
            writer.StartArray();
            for(auto turn : turns_) {
                turn.Serialize(writer);
            }
            writer.EndArray();

        writer.EndObject();
    }

private:
    int current_player_index_;
    int winning_player_index_;
    std::array<Player*, 2> players_;
    std::vector<Turn> turns_;
    BoardState current_board_;

};

#endif
