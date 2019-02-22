#ifndef QUORIDORBOT_WINDOWEDGAMERUNNER_H_
#define QUORIDORBOT_WINDOWEDGAMERUNNER_H_

#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>

#include "game.h"
#include "random_player.h"
#include "shortest_path_player.h"

using namespace std;

class WindowedGameRunner {
public:

WindowedGameRunner();
~WindowedGameRunner();

void Initialize();
void Run();
void CleanUp();

private:
void HandleInput();
void Update();
void Draw(const BoardState& board_state);

SDL_Window* window_ = nullptr;
SDL_Renderer* renderer_ = nullptr;

Game* game_ = nullptr;

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