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

void Game::Play(bool print_boards) {
    while (!TakeTurn(print_boards)) {        
    }
}

bool Game::TakeTurn(bool print_boards) {
    if (winning_player_index_ != -1) {
        return true;
    }

    auto player = players_[current_player_index_];
    auto action = player->TakeAction(current_board_);
    auto action_is_valid = ValidateAction(current_board_, current_player_index_, action, true);
    if (!action_is_valid) {
        winning_player_index_ = 1 - current_player_index_;
        cout << "Player " << winning_player_index_ << " Wins (Failing Action: " << action << ")" << endl;
        return true;
    }
    if (print_boards)
        cout << action << endl;
    auto turn = Turn(current_board_, current_player_index_, action);

    turns_.push_back(turn);
    auto new_board = BoardState(current_board_);
    action.Apply(new_board, current_player_index_);

    current_board_ = new_board;
    if (print_boards)
        std::cout << current_board_;

    if (current_board_.GetPlayerPosition(current_player_index_).y == player->GetGoalRow()) {
        winning_player_index_ = current_player_index_;
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

int Game::GetWinner() {
    return winning_player_index_;
}

int Game::GetTurnCount() {
    return turns_.size();
}
