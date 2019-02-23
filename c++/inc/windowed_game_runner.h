#ifndef QUORIDORBOT_WINDOWEDGAMERUNNER_H_
#define QUORIDORBOT_WINDOWEDGAMERUNNER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <string>

#include "game.h"
#include "random_player.h"
#include "shortest_path_player.h"

using namespace std;

enum DebugMatrixMode {
    DMM_NONE,
    DMM_DISTANCE,
    DMM_DEVIATION
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
SDL_Color font_color_ = {0, 0, 0, 255};

Game* game_ = nullptr;

Matrix distance_matrices_[2];
Matrix deviation_matrices_[2];
std::map<DebugMatrixMode, std::string> DebugMatrixModeLabelMap;
DebugMatrixMode debug_matrix_mode_ = DebugMatrixMode::DMM_NONE;
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