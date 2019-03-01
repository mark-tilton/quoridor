#ifndef QUORIDORBOT_ACTION_H_
#define QUORIDORBOT_ACTION_H_

#include "vectori.h"
#include "board_state.h"

enum ActionType {
    MOVE = 0,
    BLOCK = 1,
};

class Action {
public:
    Action(const Vectori& move_position);
    Action(const Vectori& block_position, WallOrientation block_orientation);

    virtual void Apply(BoardState& board_state, int player_index) const;

    ActionType GetType() const;
    Vectori GetMovePosition() const;
    Vectori GetBlockPosition() const;
    WallOrientation GetBlockOrientation() const;

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        if (type_ == ActionType::MOVE) {
            writer.Key("move_x");
            writer.Int(move_position_.x);

            writer.Key("move_y");
            writer.Int(move_position_.y);

            writer.Key("block_x");
            writer.Null();

            writer.Key("block_y");
            writer.Null();

            writer.Key("block_orientation");
            writer.Null();
        } 
        else {
            writer.Key("move_x");
            writer.Null();

            writer.Key("move_y");
            writer.Null();

            writer.Key("block_x");
            writer.Int(block_position_.x);

            writer.Key("block_y");
            writer.Int(block_position_.y);

            writer.Key("block_orientation");
            writer.Int(block_orientation_);
        }

        writer.EndObject();
    }

protected:
    ActionType type_;
    Vectori move_position_;
    Vectori block_position_;
    WallOrientation block_orientation_;
};

std::ostream &operator<< (std::ostream &os, const Action &a);

#endif
