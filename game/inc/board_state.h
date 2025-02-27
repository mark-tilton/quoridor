#ifndef QUORIDORBOT_BOARDSTATE_H_
#define QUORIDORBOT_BOARDSTATE_H_

#include <vector>
#include <array>
#include <optional>
#include "vectori.h"
#include "vectord.h"
#include "matrix.h"
#include "wall_orientation.h"

class Action;

class BoardState {

public:
    BoardState();
    BoardState(const BoardState& other);
    BoardState(const BoardState& other, const Action& action, const int player_index);

    WallOrientation GetWall(const Vectori& position) const;
    void SetWall(const Vectori& position, const WallOrientation value);

    Vectori GetPlayerPosition(const int player_index) const;
    void SetPlayerPosition(const int player_index, const Vectori& position);

    int GetPlayerWallCount(const int player_index) const;
    void SetPlayerWallCount(const int player_index, const int value);

    bool IsCellOccupied(const Vectori& position) const;
    bool IsPathBlocked(const Vectori& cell, const Vectori& direction) const;
    static bool IsWallIndexInBounds(const Vectori& cell);
    static bool IsCellIndexInBounds(const Vectori& cell);
    std::vector<Vectori> GetValidMoves(const Vectori& from_pos, const Vectori& opp_pos) const;
    std::vector<Vectori> GetValidMoves(int player_index) const;

    int GetPlayerDistance(const int player_index) const;
    const Matrix& GetDistanceMatrix(const int player_index) const;

    Matrix CalculateDeviationMatrix(const Matrix& distance_matrix, const Vectori& start_pos, const int max_waves = 7) const;

    static void GetWallPoints(const Vectori& cell, const Vectori& direction, Vectori& point_1, Vectori& point_2);
    static std::vector<std::vector<Vectori>> GetBlockedPaths(const Vectori& wall_position, const int orientation);

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.Key("player_1_pos_x");
        writer.Int(player_positions_[0].x);

        writer.Key("player_1_pos_y");
        writer.Int(player_positions_[0].y);

        writer.Key("player_2_pos_x");
        writer.Int(player_positions_[1].x);

        writer.Key("player_2_pos_y");
        writer.Int(player_positions_[1].y);

        writer.Key("walls");
        walls_.Serialize(writer);

        writer.EndObject();
    }

private:
    //BoardState(Vectori* player_positions, int* walls);

    Matrix CalculateDistanceMatrix(int row) const;
    std::vector<Vectori> GetAccessibleAdjacentCells(const Vectori& cell) const;
    
    Matrix walls_;
    std::array<Vectori, 2> player_positions_;
    std::array<int, 2> player_wall_counts_;
    mutable std::array<std::optional<Matrix>, 2> distance_matrices_;
};

std::ostream &operator<< (std::ostream &os, const BoardState &bs);

static Vectori directions[4] = { Vectori(1, 0), Vectori(-1, 0), Vectori(0, 1), Vectori(0, -1) };

#endif
