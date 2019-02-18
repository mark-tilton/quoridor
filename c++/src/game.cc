#include "game.h"
#include "validation.h"
#include <iostream>

using namespace std;

Game::Game(Player* player_1, Player* player_2) {
    players_ = array<Player*, 2>();
    players_[0] = player_1;
    players_[1] = player_2;    
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
    auto action = player->TakeAction(current_board_);
    auto action_is_valid = ValidateAction(current_board_, current_player_index_, action, true);
    if (!action_is_valid) {
        winning_player_index_ = 1 - current_player_index_;
        cout << "Player " << winning_player_index_ << " Wins (Invalid Move)" << endl;
        return true;
    }
    auto turn = Turn(current_board_, current_player_index_, action);

    turns_.push_back(turn);
    auto new_board = BoardState(current_board_);
    action.Apply(new_board, current_player_index_);

    current_board_ = new_board;
    //std::cout << current_board_;

    if (current_board_.GetPlayerPosition(current_player_index_).y == player->GetGoalRow()) {
        winning_player_index_ = current_player_index_;
        cout << "Player " << winning_player_index_ << " Wins (Valid Win)" << endl;
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

int Game::GetWinner() {
    return winning_player_index_;
}