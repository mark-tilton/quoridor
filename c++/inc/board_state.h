#ifndef QUORIDORBOT_BOARDSTATE_H_
#define QUORIDORBOT_BOARDSTATE_H_

#include <vector>
#include <array>
#include "vectori.h"
#include "vectord.h"
#include "matrix.h"
#include "wall_orientation.h"

class BoardState {

public:
    BoardState();
    BoardState(const BoardState& other);

    WallOrientation GetWall(const Vectori& position) const;
    void SetWall(const Vectori& position, WallOrientation value);

    Vectori GetPlayerPosition(int player_index) const;
    void SetPlayerPosition(int player_index, const Vectori& position);

    int GetPlayerWallCount(int player_index) const;
    void SetPlayerWallCount(int player_index, int value);

    bool IsCellOccupied(const Vectori& position) const;
    bool IsPathBlocked(const Vectori& cell, const Vectori& direction) const;
    bool IsWallIndexInBounds(const Vectori& cell) const;
    bool IsCellIndexInBounds(const Vectori& cell) const;
    std::vector<Vectori> GetValidMoves(const Vectori& from_pos, const Vectori& opp_pos) const;
    std::vector<Vectori> GetValidMoves(int player_index) const;
    Matrix GetDistanceMatrix(int row) const;
    Matrix GetDeviationMatrix(const Matrix& distance_matrix, const Vectori& start_pos, const int max_waves = 7) const;

    static std::vector<Vectori> GetWallPoints(const Vectori& cell, const Vectori& direction);
    static std::vector<std::vector<Vectori>> GetBlockedPaths(const Vectori& wall_position, int orientation);

private:
    //BoardState(Vectori* player_positions, int* walls);

    std::vector<Vectori> GetAccessibleAdjacentCells(const Vectori& cell) const;
    
    Matrix walls_;
    std::array<Vectori, 2> player_positions_;
    std::array<int, 2> player_wall_counts_;
};

std::ostream &operator<< (std::ostream &os, const BoardState &bs);

static Vectori directions[4] = { Vectori(1, 0), Vectori(-1, 0), Vectori(0, 1), Vectori(0, -1) };

#endif