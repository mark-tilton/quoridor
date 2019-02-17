#include "board_state.h"
#include <iostream>
#include <queue>
#include <memory>

using namespace std;

BoardState::BoardState() : walls_(Matrix(8, 8)) {
}
    
// BoardState::BoardState(Vectori* player_positions, int* walls) : 
//     player_positions_(player_positions),
//     walls_(walls) {
// }

BoardState::BoardState(const BoardState& other) : 
    walls_(Matrix(other.walls_)),
    player_positions_(other.player_positions_),
    player_wall_counts_(other.player_wall_counts_) {
}

WallOrientation BoardState::GetWall(const Vectori& position) const {
    return static_cast<WallOrientation>(walls_[position]);
}

void BoardState::SetWall(const Vectori& position, WallOrientation value) {
    walls_[position] = value;
}

Vectori BoardState::GetPlayerPosition(int player_index) const {
    return player_positions_[player_index];
}

void BoardState::SetPlayerPosition(int player_index, const Vectori& position) {
    player_positions_[player_index] = position;
}

int BoardState::GetPlayerWallCount(int player_index) const {
    return player_wall_counts_[player_index];
}

void BoardState::SetPlayerWallCount(int player_index, int value) {
    player_wall_counts_[player_index] = value;
}

bool BoardState::IsCellOccupied(const Vectori& position) const {
    return (position == player_positions_[0] || position == player_positions_[1]);
}

bool BoardState::IsPathBlocked(const Vectori& cell, const Vectori& direction) const {
    auto orientation = direction.y == 0 ? 1 : 2;
    auto points = BoardState::GetWallPoints(cell, direction);
    for (auto point : points) {
        if (IsWallIndexInBounds(point) && walls_[point] == orientation)
            return true;
    }        
    return false;
}

vector<vector<Vectori>> BoardState::GetBlockedPaths(const Vectori& wall_position, int orientation) { 
    auto middle_offset = Vectord(0.5, 0.5);
    auto c_wall = wall_position + middle_offset;
    auto direction = orientation == 1 ? Vectori(1, 0) : Vectori(0, 1);
    auto c_dir = direction / 2;
    auto perp = Vectord(c_dir.y, c_dir.x);
    auto path1 = vector<Vectori> { c_wall - perp - c_dir, c_wall - perp + c_dir };
    auto path2 = vector<Vectori> { c_wall + perp - c_dir, c_wall + perp + c_dir };
    return vector<vector<Vectori>> { path1, path2 };
}

vector<Vectori> BoardState::GetWallPoints(const Vectori& cell, const Vectori& direction) {
    auto points = vector<Vectori>();
    auto center = cell - Vectord(0.5, 0.5);
    auto cdir = direction / 2;
    auto perp = Vectord(cdir.y, cdir.x);
    auto wall = center + cdir;
    points.push_back(Vectori(wall + perp));
    points.push_back(Vectori(wall - perp));
    return points;
}

bool BoardState::IsWallIndexInBounds(const Vectori& cell) const {
    return cell.x >= 0 and cell.y >= 0 and cell.x < 8 and cell.y < 8;
}

bool BoardState::IsCellIndexInBounds(const Vectori& cell) const {
    return cell.x >= 0 and cell.y >= 0 and cell.x < 9 and cell.y < 9;
}

Matrix BoardState::GetDistanceMatrix(int row) const {
    auto matrix = Matrix(9, 9);
    matrix.SetValues(-1);
    auto cell_queue = queue<Vectori>();
    for (int x = 0; x < 9; x++) {
        auto cell = Vectori(x, row);
        matrix[cell] = 0;
        cell_queue.push(cell);
    }
    while (!cell_queue.empty()) {
        auto cell = cell_queue.front();
        cell_queue.pop();
        auto distance = matrix[cell];
        for (auto adjacent_cell : GetAccessibleAdjacentCells(cell)) {
            if (matrix[adjacent_cell] == -1) {
                matrix[adjacent_cell] = distance + 1;
                cell_queue.push(adjacent_cell);
            }
        }
    }
    return matrix;
}

Matrix BoardState::GetDeviationMatrix(const Matrix& distance_matrix, const Vectori& start_pos) const {
    unique_ptr<vector<Vectori>> current_wave(new vector<Vectori>());
    unique_ptr<vector<Vectori>> next_wave(new vector<Vectori>());

    auto deviation_matrix = Matrix(9, 9);
    deviation_matrix.SetValues(-1);
    deviation_matrix[start_pos] = 0;
    auto valid_moves = GetValidMoves(start_pos, start_pos);
    auto min_distance = -1;
    for (auto move : valid_moves) {
        next_wave->push_back(move);
        auto move_distance = distance_matrix[move];
        if (min_distance == -1 || move_distance < min_distance) {
            min_distance = move_distance;
        }
    }

    auto wave_count = 0;
    while (!next_wave->empty() && wave_count < 7) {
        wave_count += 1;
        current_wave.swap(next_wave);
        next_wave->clear();
        for (auto cell : *current_wave) {
            auto distance = distance_matrix[cell];
            deviation_matrix[cell] = distance - min_distance;
            for (auto direction : directions) {
                auto new_position = cell + direction;
                if (IsCellIndexInBounds(new_position) 
                    && deviation_matrix[new_position] == -1 
                    && !IsPathBlocked(cell, direction)) {
                    next_wave->push_back(new_position);
                }
            }
        }
        min_distance -= 1;
    }
    return deviation_matrix;
}

vector<Vectori> BoardState::GetAccessibleAdjacentCells(const Vectori& cell) const {
    auto cells = vector<Vectori>();
    for (auto direction : directions) {
        auto new_cell = cell + direction;
        if (IsCellIndexInBounds(new_cell) && !IsPathBlocked(cell, direction)) {
            cells.push_back(new_cell);
        }
    }
    return cells;
}

vector<Vectori> BoardState::GetValidMoves(const Vectori& from_pos, const Vectori& opp_pos) const {
    auto result = vector<Vectori>();
    for (auto position : GetAccessibleAdjacentCells(from_pos)) {
        if (position == opp_pos) {
            for (auto jump_position : GetValidMoves(opp_pos, opp_pos)) {
                if (jump_position != position) {
                    result.push_back(jump_position);
                }
            }
        }
        else {
            result.push_back(position);
        }
    }
    return result;
}

ostream &operator<< (ostream &os, const BoardState &bs) {
    for (int y = 0; y < 19; y++) { // 9 cells + 8 inner walls + 2 outer walls
        for (int x = 0; x < 19; x++) {
            auto cell_char = " ";
            auto is_vert_wall = x % 2 == 0;
            auto is_hori_wall = y % 2 == 0;
            if (y == 0 || y == 18)
                cell_char = "―";
            else if (x == 0 || x == 18)
                cell_char = "|";
            else if (!is_vert_wall && !is_hori_wall) { // is this a cell?
                auto cell_x = int((x - 1) / 2);
                auto cell_y = int((y - 1) / 2);
                if (bs.IsCellOccupied(Vectori(cell_x, cell_y)))
                    cell_char = "P";
                // else if (self.distance_matrix is not None) {
                //     val = self.distance_matrix[cell_x][cell_y]
                //     cell_char = ' ' if val is None else str(val % 10)
                // }
            }
            else if (is_vert_wall && is_hori_wall) { // Corners
                cell_char = "·";
            }
            else if (is_vert_wall) {
                auto wall_pos_x = (x - 2) / 2;
                auto wall_pos_y = (y - 1) / 2;
                if (bs.IsPathBlocked(Vectori(wall_pos_x, wall_pos_y), Vectori(1, 0)))
                    cell_char = "|";
            }
            else if (is_hori_wall) {
                auto wall_pos_x = (x - 1) / 2;
                auto wall_pos_y = (y - 2) / 2;
                if (bs.IsPathBlocked(Vectori(wall_pos_x, wall_pos_y), Vectori(0, 1)))
                    cell_char = "―";
            }
            os << cell_char;
        }            
        os << endl;
    }        
    return os;
};