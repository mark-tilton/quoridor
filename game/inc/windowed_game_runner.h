#ifndef QUORIDORBOT_WINDOWEDGAMERUNNER_H_
#define QUORIDORBOT_WINDOWEDGAMERUNNER_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <string>

#include "game.h"
#include "random_player.h"
#include "shortest_path_player.h"
#include "minimax_player.h"
#include "mcts_player.h"

using namespace std;

enum DebugMatrixMode {
    DMM_NONE,
    DMM_DISTANCE,
    DMM_DEVIATION
};

enum DebugMatrixTextMode {
    DMTM_OFF,
    DMTM_BOTHPLAYERS,
    DMTM_CURRENTPLAYER,
};

class WindowedGameRunner {
public:

WindowedGameRunner();
~WindowedGameRunner();

void Run();

private:
void StartNewGame();
void HandleInput();
void Update();
void Draw(const BoardState& board_state);

SDL_Window* window_ = nullptr;
SDL_Renderer* renderer_ = nullptr;
TTF_Font* font_ = nullptr;
SDL_Color font_color_ = {255, 0, 0, 255};
SDL_Color opp_font_color_ = {75, 75, 75, 255};

Game* game_ = nullptr;

Matrix wall_player_matrix_;
Matrix distance_matrices_[2];
Matrix deviation_matrices_[2];
DebugMatrixMode debug_matrix_mode_ = DebugMatrixMode::DMM_NONE;
std::map<DebugMatrixMode, std::string> DebugMatrixModeLabelMap;
DebugMatrixTextMode debug_matrix_text_mode_ = DebugMatrixTextMode::DMTM_BOTHPLAYERS;
std::map<DebugMatrixTextMode, std::string> DebugMatrixTextModeLabelMap;
int debug_matrix_player_ = 0;

BoardState board_state_;
bool keys_allow_repeat_[SDL_NUM_SCANCODES];
bool keys_[SDL_NUM_SCANCODES];
bool auto_play_next_game_ = false;
bool is_paused_ = false;
int current_turn_index_ = -1;
int last_turn_index_ = -1;
int max_fps_ = 10;
Uint32 last_update_ = 0;

};

#endif