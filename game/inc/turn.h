#ifndef QUORIDORBOT_TURN_H_
#define QUORIDORBOT_TURN_H_

#include "board_state.h"
#include "action.h"

class Turn {
public:
    Turn(const BoardState& board_state, int player_index, const Action& action);

    const BoardState& GetBoardState() const;
    int GetPlayerIndex() const;
    const Action& GetAction() const;

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.Key("player_index");
        writer.Int(player_index_);

        writer.Key("board_state");
        board_state_.Serialize(writer);

        writer.Key("action");
        action_.Serialize(writer);

        writer.EndObject();
    }

private:
    BoardState board_state_;
    int player_index_;
    Action action_;
};

#endif