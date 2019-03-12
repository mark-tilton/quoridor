#ifndef QUORIDORBOT_TURN_H_
#define QUORIDORBOT_TURN_H_

#include "board_state.h"
#include "action.h"
#include <optional>

class Turn {
public:
    Turn(const BoardState& board_state, int player_index);
    Turn(const BoardState& board_state, int player_index, const Action& action);

    BoardState GetBoardState() const;
    int GetPlayerIndex() const;
    std::optional<Action> GetAction() const;

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.Key("player_index");
        writer.Int(player_index_);

        writer.Key("board_state");
        board_state_.Serialize(writer);

        writer.Key("action");
        if (action_.has_value()) {
            action_.value().Serialize(writer);
        }
        else {
            writer.Null();
        }

        writer.EndObject();
    }

private:
    BoardState board_state_;
    int player_index_;
    std::optional<Action> action_;
};

#endif