#ifndef QUORIDORBOT_BOARDSTATE_H_
#define QUORIDORBOT_BOARDSTATE_H_

#include <vector>
#include "vectori.h"
#include "vectord.h"
#include "matrix.h"

class BoardState {

public:
    BoardState();

    int GetWall(const Vectori& position) const;
    void SetWall(const Vectori& position, int value);

    Vectori GetPlayerPosition(int player_index) const;
    void SetPlayerPosition(int player_index, const Vectori& position);

    bool IsCellOccupied(const Vectori& position) const;
    bool IsPathBlocked(const Vectori& cell, const Vectori& direction) const;
    bool IsWallIndexInBounds(const Vectori& cell) const;
    bool IsCellIndexInBounds(const Vectori& cell) const;
    Matrix GetDistanceMatrix(int row) const;
    Matrix GetDeviationMatrix(const Matrix& distance_matrix, const Vectori& start_pos) const;

    static std::vector<Vectori> GetWallPoints(const Vectori& cell, const Vectori& direction);
    static std::vector<std::vector<Vectori>> GetBlockedPaths(const Vectori& wall_position, int orientation);

private:
    //BoardState(Vectori* player_positions, int* walls);

    std::vector<Vectori> GetAccessibleAdjacentCells(const Vectori& cell) const;
    std::vector<Vectori> GetValidMoves(const Vectori& from_pos, const Vectori& opp_pos) const;
    
    Vectori player_positions_[2];
    int player_wall_counts_[2];
    Matrix walls_;

};

std::ostream &operator<< (std::ostream &os, const BoardState &bs);

static Vectori directions[4] = { Vectori(1, 0), Vectori(-1, 0), Vectori(0, 1), Vectori(0, -1) };

#endif