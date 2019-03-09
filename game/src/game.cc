#include "game.h"
#include "validation.h"
#include <iostream>

using namespace std;

Game::Game(Player* player_1, Player* player_2) {
    players_ = array<Player*, 2>();
    players_[0] = player_1;
    players_[1] = player_2;
    player_1->SetIndex(0);
    player_2->SetIndex(1);
    Reset();
}

void Game::Play(const bool print_boards) {
    while (!TakeTurn(print_boards)) {        
    }
}

bool Game::TakeTurn(const bool print_boards) {
    if (winning_player_index_ != -1) {
        return true;
    }

    auto player = players_[current_player_index_];
    auto action = player->TakeAction(current_board_);
    const auto action_is_valid = ValidateAction(current_board_, current_player_index_, action, true);
    if (!action_is_valid) {
        winning_player_index_ = 1 - current_player_index_;
        cout << "Player " << winning_player_index_ << " Wins (Failing Action: " << action << ")" << endl;
        return true;
    }
    if (print_boards)
        cout << action << endl;
    const auto new_board = BoardState(current_board_, action, current_player_index_);

    const auto turn = Turn(current_board_, current_player_index_, action);
    turns_.push_back(turn);

    current_board_ = new_board;
    if (print_boards)
        std::cout << current_board_;

    if (current_board_.GetPlayerDistance(current_player_index_) == 0) {
        winning_player_index_ = current_player_index_;
        // Add turn with no action to capture final board state
        auto turn = Turn(new_board, 1 - current_player_index_);
        turns_.push_back(turn);
        //cout << "Player " << winning_player_index_ << " Wins" << endl;
        return true;
    }

    current_player_index_ = 1 - current_player_index_;
    return false;
}

void Game::Reset() {
    turns_.clear();
    current_player_index_ = 0;
    winning_player_index_ = -1;
    current_board_ = BoardState();
}

int Game::GetWinner() const {
    return winning_player_index_;
}

Turn Game::GetTurn(const int index) const {
    return turns_[index];
}

int Game::GetTurnCount() const {
    return turns_.size();
}
