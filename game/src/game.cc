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

void Game::Play() {
    while (!TakeTurn()) {        
    }
}

bool Game::TakeTurn() {
    if (winning_player_index_ != -1) {
        return true;
    }

    auto player = players_[current_player_index_];
    const auto action = player->TakeAction(current_board_);
    const auto action_is_valid = ValidateAction(current_board_, current_player_index_, action, true);
    if (!action_is_valid) {
        cout << "Player " << current_player_index_ << " Made An Invalid Move! (" << action << ")" << endl;
        return true;
    }
    
    turns_.emplace_back(current_board_, current_player_index_, action);

    current_board_ = BoardState(current_board_, action, current_player_index_);

    if (current_board_.GetPlayerDistance(current_player_index_) == 0) {
        winning_player_index_ = current_player_index_;
        // Add turn with no action to capture final board state
        turns_.emplace_back(current_board_, 1 - current_player_index_);
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
