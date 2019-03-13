#include "game.h"
#include "validation.h"
#include <iostream>

using namespace std;

Game::Game(Player* player_1, Player* player_2, bool validate_actions, bool store_turns, const BoardState& initial_board) :
    validate_actions_(validate_actions),
    store_turns_(store_turns) {
    players_ = array<Player*, 2>();
    players_[0] = player_1;
    players_[1] = player_2;
    player_1->SetIndex(0);
    player_2->SetIndex(1);
    Reset(initial_board);
}

void Game::Play() {
    while (!TakeTurn()) {        
    }
}

bool Game::TakeTurn() {
    if (winning_player_index_ != -1) {
        return true;
    }

    auto current_player = players_[current_player_index_];
    const auto action = current_player->TakeAction(current_board_);
    
    if (validate_actions_ && !ValidateAction(current_board_, current_player_index_, action, true)) {
        cout << "Player " << current_player_index_ << " Made An Invalid Move! (" << action << ")" << endl;
        return true;
    }
    
    current_board_ = BoardState(current_board_, action, current_player_index_);
		
    if (store_turns_)
        turns_.emplace_back(current_board_, current_player_index_, action);
    turn_count_ += 1;

    if (current_board_.GetPlayerDistance(current_player_index_) == 0) {
        winning_player_index_ = current_player_index_;
        return true;
    }

    current_player_index_ = 1 - current_player_index_;
    return false;
}

void Game::Reset(const BoardState& initial_board) {
    if (store_turns_) {
	    turns_.clear();
    }
    turn_count_ = 0;
    current_player_index_ = 0;
    winning_player_index_ = -1;
    initial_board_ = initial_board;
    current_board_ = initial_board;
}

int Game::GetWinner() const {
    return winning_player_index_;
}

Turn Game::GetTurn(const int index) const {
    return turns_[index];
}

int Game::GetTurnCount() const {
    return turn_count_;
}

const BoardState& Game::GetBoardState() const {
    return current_board_;
}

const BoardState& Game::GetInitialBoardState() const {
    return initial_board_;
}
